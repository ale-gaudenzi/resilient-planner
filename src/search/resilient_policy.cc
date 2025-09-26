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
    ResilientNodeFormula initial_node = ResilientNodeFormula(initial_certiticate.get_formula(), K, set<Operator>(), initial_certiticate.get_next_operator());


    open.push_back(make_pair(initial_node, initial_certiticate));
    while (!open.empty())
    {
        ResilientNodeFormula current_node = open.front().first;
        ResilientNodeFormula current_certificate = open.front().second;

        set<Operator> current_node_forbidden = current_node.get_pi();
        open.pop_front();
        
        if (current_node.get_k() == 0)
        {
            ResilientNodeFormula successor_formula = resilient_nodes_formula_by_k[0][current_certificate][0];
            PartialState successor = successor_formula.get_formula();
            ResilientNodeFormula successor_node = ResilientNodeFormula(successor, 0, current_node_forbidden, successor_formula.get_next_operator());
            PolicyNode policy_node = PolicyNode(current_node, 0, current_node_forbidden, successor_node.get_id(), 0);

            policy[current_node.get_id()] = policy_node;

            if (!goal.is_implied(successor))
            {
                open.push_back(make_pair(successor_node, successor_formula));
            }
        }
        else
        {
            ResilientNodeFormula first = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][0];
            ResilientNodeFormula second = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][1];

            current_node_forbidden.insert(current_certificate.get_next_operator());

            if(first.get_k() >= current_node.get_k()){

                ResilientNodeFormula current_node_lower_level = ResilientNodeFormula(second.get_formula(), current_node.get_k() - 1, current_node_forbidden, second.get_next_operator());
                ResilientNodeFormula successor_node = ResilientNodeFormula(first.get_formula(), current_node.get_k(), current_node.get_pi(), first.get_next_operator());

                PolicyNode policy_node = PolicyNode(current_node, current_node.get_k(), current_node.get_pi(), successor_node.get_id(), current_node_lower_level.get_id());

                policy[current_node.get_id()] = policy_node;

                PartialState successor_partial = successor_node.get_formula();
                open.push_back(make_pair(current_node_lower_level, second));

                if(!goal.is_implied(successor_partial)){
                    open.push_back(make_pair(successor_node, first));
                }
            }
            else{
                ResilientNodeFormula current_node_lower_level = ResilientNodeFormula(first.get_formula(), current_node.get_k() - 1, current_node_forbidden, first.get_next_operator());
                ResilientNodeFormula successor_node = ResilientNodeFormula(second.get_formula(), current_node.get_k(), current_node.get_pi(), second.get_next_operator());

                PolicyNode policy_node = PolicyNode(current_node, current_node.get_k(), current_node.get_pi(),  successor_node.get_id(), current_node_lower_level.get_id());

                policy[current_node.get_id()] = policy_node;

                PartialState successor_partial = successor_node.get_formula();
                open.push_back(make_pair(current_node_lower_level, first));
                if(!goal.is_implied(successor_partial)){
                    open.push_back(make_pair(successor_node, second));
                }
            }

        }
        i++;
    }

    // set<int> ids;
    // list<ResilientNodeFormula> another_open;
    // another_open.push_back(initial_certiticate);
    // ids.insert(initial_certiticate.get_id());
    // while (!another_open.empty())
    // {
    //     ResilientNodeFormula current_certificate = another_open.front();
    //     another_open.pop_front();
    //     if (current_certificate.get_k() == 0)
    //     {
    //         ResilientNodeFormula successor_certificate = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][0];
    //         if (policy.count(successor_certificate) > 0) {
    //             ids.insert(successor_certificate.get_id());
    //             another_open.push_back(successor_certificate);
    //         }
    //     }
    //     else
    //     {
    //         ResilientNodeFormula first = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][0];
    //         ResilientNodeFormula second = resilient_nodes_formula_by_k[current_certificate.get_k()][current_certificate][1];
    //         if (policy.count(first) > 0) {
    //             ids.insert(first.get_id());
    //             another_open.push_back(first);
    //         }
    //         if (policy.count(second) > 0) {
    //             ids.insert(second.get_id());
    //             another_open.push_back(second);
    //         }
    //     }

    // }

    // cout << "Dimensione totale nodi: " << ids.size() << endl;

    // for (auto it = policy.begin(); it != policy.end(); )
    // {
    //     const ResilientNodeFormula& key = it->first;
    //     if(ids.count(key.get_id())){
    //         ++it;
    //     }else{
    //         it = policy.erase(it);
    //     }
    // }

    if (not_found_counter > 0)
        cout << "\n\nResilient policy created, but " << not_found_counter << " nodes not found." << endl;
    else
        cout << "\n\nResilient policy created, " << policy.size() << " nodes found." << endl;

}