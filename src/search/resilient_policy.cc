#include "resilient_policy.h"
#include "ext/json.h"

using namespace json;
using namespace std;

/// Extract the resilient policy from the global policy.
/// @param initial_state The first state of the policy.
/// @param goal The last state of the policy.
/// @param K The resilience parameter.
/// @param resilient_nodes The set of resilient nodes.
void ResilientPolicy::extract_policy(State initial_state, ResilientNodeFormula initial_certiticate, PartialState goal, int K, std::tr1::unordered_map<int, std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>> >  resilient_nodes_formula_by_k)
{
    StateRegistry *registry = const_cast<StateRegistry *>(&initial_state.get_registry());
    int not_found_counter = 0;
    int i = 0;
    list<pair<ResilientNode, ResilientNodeFormula> > open;
    open.push_back(make_pair(ResilientNode(initial_state, K, set<Operator>()), initial_certiticate));
    while (!open.empty())
    {
        bool found = false;
        ResilientNode current_node = open.front().first;
        ResilientNodeFormula current_certificate = open.front().second;
        set<Operator> current_node_forbidden = current_node.get_deactivated_op();
        if (policy.find(current_node) != policy.end()){
            found = true;
            list<ResilientNode> to_delete_list;
            ResilientNode to_delete = current_node;
            to_delete_list.push_back(to_delete);
            while (to_delete_list.size() > 0){
                ResilientNode to_delete_node = to_delete_list.front();
                to_delete_list.pop_front();
                if(policy.find(to_delete_node)!= policy.end()){
                    Operator op = policy[to_delete];
                    policy.erase(to_delete_node);
                    State successor = registry->get_successor_state(to_delete.get_state(), op);
                    PartialState successor_partial = PartialState(successor);
                    if(!goal.is_implied(successor_partial)){
                    	to_delete_list.push_back(ResilientNode(successor, to_delete_node.get_k(), to_delete_node.get_deactivated_op()));
                    }
                    if(to_delete_node.get_k()>0){
                        set<Operator> to_delete_node_forbidden = to_delete_node.get_deactivated_op();
                        to_delete_node_forbidden.insert(op);
                        to_delete_list.push_back(ResilientNode(to_delete_node.get_state(), to_delete_node.get_k() - 1, to_delete_node_forbidden));
                    }
                }
            }
        }
        else
        {
            found = true;
            open.pop_front();
            policy[current_node] = current_certificate.get_next_operator();
            State successor = registry->get_successor_state(current_node.get_state(), current_certificate.get_next_operator());
            if (current_node.get_k() == 0)
            {
                ResilientNode successor_node = ResilientNode(successor, current_node.get_k(), current_node.get_deactivated_op());
                PartialState successor_partial = PartialState(successor);
                if(!goal.is_implied(successor_partial)){
                    ResilientNodeFormula successor_formula = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][0];
                    open.push_back(make_pair(successor_node, successor_formula));
                }
            }
            else
            {
                ResilientNode successor_node = ResilientNode(successor, current_node.get_k(), current_node.get_deactivated_op());
                PartialState current_partial = PartialState(current_node.get_state());
                PartialState successor_partial = PartialState(successor);
                current_node_forbidden.insert(current_certificate.get_next_operator());
                ResilientNode current_node_lower_level = ResilientNode(current_node.get_state(), current_node.get_k() - 1, current_node_forbidden);
                ResilientNodeFormula first = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][0];
                ResilientNodeFormula second = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][1];
                if(first.get_formula().is_model(current_partial) && first.get_k() >= current_node_lower_level.get_k() && second.get_k() >= successor_node.get_k()){
                    open.push_back(make_pair(current_node_lower_level, first));
                    if(!goal.is_implied(successor_partial)){
                        open.push_back(make_pair(successor_node, second));
                    }
                }
                else if(second.get_formula().is_model(current_partial) && first.get_k() >= successor_node.get_k() && first.get_k() >= current_node_lower_level.get_k()){
                    open.push_back(make_pair(current_node_lower_level, second));
                    if(!goal.is_implied(successor_partial)){
                        open.push_back(make_pair(successor_node, first));
                    }
                }
                else{
                  continue;
                }
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

