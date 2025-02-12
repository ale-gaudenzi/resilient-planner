#include "resilient_node_formula.h"

#include <tr1/unordered_map>
using std::tr1::hash;

using namespace std;

/// @brief Constructor for ResilientNode that takes all the parameters needed to create a node
/// @param formula_ Full state of the node
/// @param k_ Number of operators that can still fail
/// @param deactivated_op_ Deactivated operators
ResilientNodeFormula::ResilientNodeFormula(PartialState formula_, int k_, std::set<Operator> deactivated_op_, std::vector<Operator> pi_) : formula(formula_), k(k_), deactivated_op(deactivated_op_), pi(pi_)
{
    string op_value;
    string op_pi_value;

    if (deactivated_op.size() != 0)
        for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
            op_value += it->get_name();
    else
        op_value = "";

    string state_value;

    for (int i = 0; i < g_variable_domain.size(); i++)
    {
        if (-1 != formula[i])
        {
            const string &fact_name = g_fact_names[i][formula[i]];
            if (fact_name != "<none of those>")
                state_value += fact_name;
        }
    }

    if (pi.size() != 0)
        for (auto pi_el : pi)
            op_pi_value += pi_el.get_name();
    else
        op_pi_value = "";

    std::tr1::hash<string> hasher;
    int hash = hasher(op_value + state_value + op_pi_value);
    id = hash;
}


/// @brief Dump node information to the standard output
void ResilientNodeFormula::dump() const
{
    cout << "Node: " << id << endl;
    formula.dump_pddl();
    cout << "k: " << k << endl;
    cout << "deactivated_op: " << endl;
    for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
        cout << it->get_nondet_name() << endl;
    cout << "pi: " << endl;    
    for (Operator op_pi: pi)
        cout << op_pi.get_name() << endl;

}
