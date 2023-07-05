#ifndef RESILIENT_POLICY_H
#define RESILIENT_POLICY_H

#include "operator.h"
#include "resilient_node.h"
#include "policy-repair/policy.h"
#include "globals.h"

#include <set>

using namespace std;

class ResilientPolicy
{

public:
    map<ResilientNode, Operator> policy;

    Operator get_successor(ResilientNode node);
    void add_item(ResilientNode node, Operator op);
    void extract_policy(State initial_state, PartialState goal, int K, set<ResilientNode> resilient_set);
    void dump();
    void dump_json();
};

#endif
