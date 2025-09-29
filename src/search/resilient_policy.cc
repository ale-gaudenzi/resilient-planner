#include "resilient_policy.h"
#include "ext/json.h"

using namespace json;
using namespace std;

/// Extract the resilient policy from the global policy.
/// @param initial_state The first state of the policy.
/// @param goal The last state of the policy.
/// @param K The resili ence parameter.
/// @param resilient_nodes The set of resilient nodes.
void ResilientPolicy::extract_policy(State initial_state, ResilientNodeFormula initial_certiticate, PartialState goal, int K, std::tr1::unordered_map<int, std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>> >  resilient_nodes_formula_by_k)
{
    int not_found_counter = 0;
    int i = 0;
    StateRegistry *registry = const_cast<StateRegistry *>(&initial_state.get_registry());
    list<pair<ResilientNode, ResilientNodeFormula> > open;
    ResilientNode initial_node = ResilientNode(initial_state, K, set<Operator>());
    open.push_back(make_pair(initial_node, initial_certiticate));
    while (!open.empty())
    {
        bool found = false;
        ResilientNode current_node = open.front().first;
        ResilientNodeFormula current_certificate = open.front().second;
        set<Operator> current_node_forbidden = current_node.get_deactivated_op();
        found = true;
        open.pop_front();
        Operator next_op = current_certificate.get_next_operator();
        policy[current_node] = next_op;
        State successor = registry->get_successor_state(current_node.get_state(), next_op);
        PartialState successor_p = PartialState(successor);
        PartialState current_p = PartialState(current_node.get_state());

        if (current_node.get_k() == 0)
        {
            ResilientNodeFormula successor_formula = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][0];
            if (!goal.is_implied(successor_p))
            {
                ResilientNode successor_node = ResilientNode(successor, current_node.get_k(), current_node.get_deactivated_op());
                open.push_back(make_pair(successor_node, successor_formula));
            }
        }
        else
        {
            current_node_forbidden.insert(current_certificate.get_next_operator());

            ResilientNodeFormula first = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][0];
            ResilientNodeFormula second = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][1];

            ResilientNode current_node_lower_level = ResilientNode(current_node.get_state(), current_node.get_k() - 1, current_node_forbidden);
            ResilientNode successor_node_same_level = ResilientNode(successor, current_node.get_k(), current_node.get_deactivated_op());
            if(first.get_k() >= successor_node_same_level.get_k() && first.get_formula().is_model(successor_p)){
                	open.push_back(make_pair(current_node_lower_level, second));
                if(!goal.is_implied(successor_p)){
                 	open.push_back(make_pair(successor_node_same_level, first));
                }
            }
            else if(second.get_k() >= successor_node_same_level.get_k() && second.get_formula().is_model(successor_p))
            {
                open.push_back(make_pair(current_node_lower_level, first));
                if(!goal.is_implied(successor_p)){
                	open.push_back(make_pair(successor_node_same_level, second));
                }
            }else{
                found = false;
            }
        }
        i++;
        if (!found)
        {
            cout << "\nNode not found at iteration #" << i << endl;
            current_node.dump();
            cout << "next_actions:" << endl;
            not_found_counter += 1;
        }
    }
    if (not_found_counter > 0)
        cout << "\n\nResilient policy created, but " << not_found_counter << " nodes not found." << endl;
    else
        cout << "\n\nResilient policy created, " << policy.size() << " nodes found." << endl;

}