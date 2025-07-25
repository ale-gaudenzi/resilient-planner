#include "policy_node.h"

#include <tr1/unordered_map>
using std::tr1::hash;

using namespace std;
/// @brief Constructor for ResilientNode that takes all the parameters needed to create a node
/// @param resilient_node_formula_ A resilient node formula
/// @param op Related operation
PolicyNode::PolicyNode(ResilientNodeFormula resilient_node_formula_, Operator op_) : resilient_node_formula(resilient_node_formula_), op(op_)
{
    std::tr1::hash<string> hasher;
    int hash = hasher(op.get_name() + std::to_string(resilient_node_formula.get_id()));
    id = hash;
}

PolicyNode::PolicyNode(): resilient_node_formula(PartialState(), -1), op(),id(-1) {}

/// @brief Dump node information to the standard output
void PolicyNode::dump() const
{
    cout << "Resilient Formula: " << id << endl;
    resilient_node_formula.dump();
    cout << "operator: " << endl;    
    cout << op.get_name() << endl;
}
