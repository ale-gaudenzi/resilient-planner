#include "resilient_policy.h"
#include "ext/json.h"

using namespace json;
using namespace std;

/// Extract the resilient policy from the global policy.
/// @param initial_state The first state of the policy.
/// @param goal The last state of the policy.
/// @param K The resilience parameter.
/// @param resilient_nodes The set of resilient nodes.
void ResilientPolicy::extract_policy(ResilientNodeFormula initial_certiticate, PartialState goal, int K, std::tr1::unordered_map<int, std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>> >  resilient_nodes_formula_by_k)
{
    int not_found_counter = 0;
    int i = 0;
    list<pair<ResilientNodeFormula, ResilientNodeFormula> > open;
    ResilientNodeFormula initial_node = ResilientNodeFormula(initial_certiticate.get_formula(), K, set<Operator>(), initial_certiticate.get_next_operator(), initial_certiticate.get_id());
    open.push_back(make_pair(initial_node, initial_certiticate));
    while (!open.empty())
    {
        bool found = false;
        ResilientNodeFormula current_node = open.front().first;
        ResilientNodeFormula current_certificate = open.front().second;
        set<Operator> current_node_forbidden = current_node.get_pi();
        found = true;
        open.pop_front();
        if (current_node.get_k() == 0)
        {
            ResilientNodeFormula successor_formula = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][0];
            PartialState successor = successor_formula.get_formula();
            policy[current_node]["current_level_resiliency"] = successor_formula.get_id();
            if (!goal.is_implied(successor))
            {
                ResilientNodeFormula successor_node = ResilientNodeFormula(successor, current_node.get_k(), current_node.get_pi(), successor_formula.get_next_operator(), successor_formula.get_id());
                open.push_back(make_pair(successor_node, successor_formula));
            }
        }
        else
        {
            ResilientNodeFormula first = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][0];
            ResilientNodeFormula second = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][1];

            current_node_forbidden.insert(current_certificate.get_next_operator());

            if(first.get_k() >= current_node.get_k()){
                ResilientNodeFormula current_node_lower_level = ResilientNodeFormula(second.get_formula(), current_node.get_k() - 1, current_node_forbidden, second.get_next_operator(), second.get_id());
                ResilientNodeFormula successor_node = ResilientNodeFormula(first.get_formula(), current_node.get_k(), current_node.get_pi(), first.get_next_operator(), first.get_id());
                PartialState successor_partial = successor_node.get_formula();
                policy[current_node]["current_level_resiliency"] = first.get_id();
                policy[current_node]["lower_level_resiliency"] = second.get_id();
                open.push_back(make_pair(current_node_lower_level, second));
                if(!goal.is_implied(successor_partial)){
                    open.push_back(make_pair(successor_node, first));
                }
            }
            else{
                ResilientNodeFormula current_node_lower_level = ResilientNodeFormula(first.get_formula(), current_node.get_k() - 1, current_node_forbidden, first.get_next_operator(), first.get_id());
                ResilientNodeFormula successor_node = ResilientNodeFormula(second.get_formula(), current_node.get_k(), current_node.get_pi(), second.get_next_operator(), second.get_id());
                policy[current_node]["current_level_resiliency"] = second.get_id();
                policy[current_node]["lower_level_resiliency"] = first.get_id();
                PartialState successor_partial = successor_node.get_formula();
                open.push_back(make_pair(current_node_lower_level, first));
                if(!goal.is_implied(successor_partial)){
                    open.push_back(make_pair(successor_node, second));
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