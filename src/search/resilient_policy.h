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
    map<ResilientNodeFormula, std::tr1::unordered_map<std::string, int>> policy;

public:
    void extract_policy(ResilientNodeFormula initial_certiticate, PartialState goal, int K, std::tr1::unordered_map<int, std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>>> resilient_nodes_formula_by_k);
    map<ResilientNodeFormula, std::tr1::unordered_map<string, int>> get_policy() { return policy; }
};

#endif
