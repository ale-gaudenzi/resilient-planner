#ifndef RESILIENT_POLICY_H
#define RESILIENT_POLICY_H

#include "operator.h"
#include "resilient_node.h"
#include "policy_node.h"
#include "resilient_node_formula.h"
#include "policy.h"
#include "globals.h"

#include <tr1/unordered_map>
#include <set>

using namespace std;

class ResilientPolicy
{
private:
    map<ResilientNode, Operator> policy;

public:
    void extract_policy(State initial_state, PartialState goal, int K, std::set<PolicyNode> resilient_set, std::tr1::unordered_map<int, ResilientNodeFormula> resilient_nodes,std::tr1::unordered_map<string, PolicyNode> macro_to_op);
    Operator get_successor(ResilientNode node) { return policy.find(node)->second; };
    map<ResilientNode, Operator> get_policy() { return policy; }
};

#endif
