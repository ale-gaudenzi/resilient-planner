#include "policy_node.h"

#include <tr1/unordered_map>
using std::tr1::hash;

using namespace std;
/// @brief Constructor for ResilientNode that takes all the parameters needed to create a node
/// @param resilient_node_formula_ A resilient node formula
/// @param op Related operation
PolicyNode::PolicyNode(ResilientNodeFormula certificate_, int k_, set<Operator> forbidden_actions_, int current_level_resiliency_, int lower_level_resiliency_) : certificate(certificate_), k(k_), forbidden_actions(forbidden_actions_), current_level_resiliency(current_level_resiliency_), lower_level_resiliency(lower_level_resiliency_)
{
    std::tr1::hash<string> hasher;
    string forbidden_op_value;
 
    if (forbidden_actions.size() != 0)
    {
        for (set<Operator>::iterator it = forbidden_actions_.begin(); it != forbidden_actions_.end(); it++)
            forbidden_op_value += it->get_name();
    }
    else
    {
        forbidden_op_value = "";
    }

    int hash = hasher(std::to_string(certificate.get_id()) + forbidden_op_value + std::to_string(current_level_resiliency_) + std::to_string(lower_level_resiliency_));
    id = hash;
}
