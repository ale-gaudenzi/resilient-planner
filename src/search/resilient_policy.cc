#include "resilient_policy.h"
#include "ext/json.h"

using namespace json;
using namespace std;

/// Extract the resilient policy from the global policy.
/// @param initial_state The first state of the policy.
/// @param goal The last state of the policy.
/// @param K The resilience parameter.
/// @param resilient_nodes The set of resilient nodes.
void ResilientPolicy::extract_policy(State initial_state, PartialState goal, int K, std::set<PolicyNode> policy_node, std::tr1::unordered_map<int, ResilientNodeFormula> resilient_nodes_formula, std::tr1::unordered_map<string, PolicyNode> macro_to_op)
{
    
    int not_found_counter = 0;
    int i = 0;
    list<ResilientNode> open;
    open.push_back(ResilientNode(initial_state, K, set<Operator>()));
    list<PolicyItem *> current_policy = g_policy->get_items();

    while (!open.empty())
    {
        bool found = false;

        ResilientNode node = open.front();
        set<Operator> current_node_forbidden = node.get_deactivated_op();
        open.pop_front();
        StateRegistry *registry = const_cast<StateRegistry *>(&(node.get_state()).get_registry());
        PartialState state = (PartialState)node.get_state();

        if (policy.find(node) != policy.end())
        {
            found = true;
        }
        else
        {
            std::vector<std::pair<int, Operator>>  appli_ops;
            for (std::set<PolicyNode>::iterator it = policy_node.begin(); it != policy_node.end(); ++it)
            {
                PolicyNode p_node = (*it);
                ResilientNodeFormula current_node_formula = (*it).get_resilient_node_formula();
                Operator possible_app_op = (*it).get_operator();
                if(current_node_formula.get_formula().is_implied(state) && current_node_formula.get_k() == node.get_k()){
                  	set<Operator> pi = current_node_formula.get_pi();
                	bool successor_same_level_is_valid = true;
                  	for (std::set<Operator>::iterator it_current_forbidden_operator = current_node_forbidden.begin(); it_current_forbidden_operator != current_node_forbidden.end(); ++it_current_forbidden_operator)
                	{
						for (std::set<Operator>::iterator it_pi_operator = pi.begin(); it_pi_operator != pi.end(); ++it_pi_operator)
                		{
                        	if ((*it_current_forbidden_operator) == (*it_pi_operator)) {
                    	    	successor_same_level_is_valid = false;
                    		}
                		}
                	}
                    if(successor_same_level_is_valid){
                    	if(possible_app_op.get_name().find("macro_") != std::string::npos){
                            p_node = macro_to_op[possible_app_op.get_name()];
                            appli_ops.push_back(make_pair(pi.size(), p_node.get_operator()));
                        }else{
                  		    appli_ops.push_back(make_pair(pi.size(), possible_app_op));
                    	}
                    }
                }
            }
            std::sort(appli_ops.begin(), appli_ops.end());
            for (size_t i = 0; i < appli_ops.size(); ++i) {
                bool current_level = false;
                bool lower_level = false;
                Operator app_op = appli_ops[i].second;
                if(app_op.is_applicable(node.get_state())){
                    State successor = registry->get_successor_state(node.get_state(), app_op);
                    ResilientNode successor_node = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>
                    PartialState successor_partial = (PartialState)successor;
                    if(node.get_k() == 0 || app_op.is_safe()){
                        for (std::tr1::unordered_map<int, ResilientNodeFormula>::iterator it = resilient_nodes_formula.begin();  it != resilient_nodes_formula.end(); ++it){
                            ResilientNodeFormula res = it->second;
                            PartialState formula = res.get_formula();
                            if ((formula.is_implied(successor_partial) && node.get_k() == res.get_k()) || goal.is_implied(successor_partial))
                            {
                                found = true;
                                policy[node] = app_op;
                                if (!goal.is_implied(successor_partial)){
                                    open.push_back(successor_node);
                                }
                                break;
                            }
                        }
                        if(found){
                            break;
                        }
                    }else{
                        std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
                        forbidden_plus_current.insert(app_op);
                        ResilientNode current_without_action = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); // <s, k-1, V / a>
                        for (std::tr1::unordered_map<int, ResilientNodeFormula>::iterator it = resilient_nodes_formula.begin();  it != resilient_nodes_formula.end(); ++it){
                            ResilientNodeFormula res = it->second;
                            PartialState formula = res.get_formula();
                            if(!current_level){
                            	if((formula.is_implied(successor_partial) && node.get_k() == res.get_k()) || goal.is_implied(successor_partial)){
                               		current_level = true;
                            	    for (std::set<Operator>::iterator it_successor_forbidden_operator = current_node_forbidden.begin(); it_successor_forbidden_operator != current_node_forbidden.end(); ++it_successor_forbidden_operator){
                                	    set<Operator> successor_pi = res.get_pi();
										for (std::set<Operator>::iterator it_successor_pi_operator = successor_pi.begin(); it_successor_pi_operator != successor_pi.end(); ++it_successor_pi_operator){
                        					if ((*it_successor_forbidden_operator) == (*it_successor_pi_operator)) {
                    	    					current_level = false;
                    						}
                                        }
                					}
                				}
                           	}
                            if(!lower_level){
                           		if(formula.is_implied(state) &&  res.get_k() == (node.get_k() - 1)){
                           	    	lower_level = true;
                                	for (std::set<Operator>::iterator it_current_forbidden_operator = current_node_forbidden.begin(); it_current_forbidden_operator != current_node_forbidden.end(); ++it_current_forbidden_operator){
                                    	set<Operator> lower_level_pi = res.get_pi();
										for (std::set<Operator>::iterator it_lower_level_pi_operator = lower_level_pi.begin(); it_lower_level_pi_operator != lower_level_pi.end(); ++it_lower_level_pi_operator){
                        					if ((*it_current_forbidden_operator) == (*it_lower_level_pi_operator)) {
                    	    					lower_level = false;
                    						}
                						}
                                    }
                				}
                           	}
                            if (current_level && lower_level){
                                found = true;
                                policy[node] = app_op;
                                open.push_back(current_without_action);
                                if (!goal.is_implied(successor_partial))
                                    open.push_back(successor_node);
                                break;
                            }
                        }
                        if(found){
                            break;
                        }
                    }
                }
            }
        }
        i++;

        if (!found)
        {
            cout << "\nNode not found at iteration #" << i << endl;
            node.dump();
            cout << "next_actions:" << endl;
            not_found_counter += 1;
        }
    }

    if (not_found_counter > 0)
        cout << "\n\nResilient policy created, but " << not_found_counter << " nodes not found." << endl;
    else
        cout << "\n\nResilient policy created, " << policy.size() << " nodes found." << endl;
}



// void ResilientPolicy::extract_policy(State initial_state, PartialState goal, int K, std::set<PolicyNode> policy_node, std::tr1::unordered_map<int, ResilientNodeFormula> resilient_nodes_formula, std::tr1::unordered_map<string, Operator> macro_to_op)
// {
//     int not_found_counter = 0;
//     int i = 0;

//     list<ResilientNode> open;
//     open.push_back(ResilientNode(initial_state, K, set<Operator>()));
//     list<PolicyItem *> current_policy = g_policy->get_items();

//     while (!open.empty())
//     {
//         bool found = false;

//         ResilientNode node = open.front();
//         open.pop_front();
//         StateRegistry *registry = const_cast<StateRegistry *>(&(node.get_state()).get_registry());
//         PartialState state = (PartialState)node.get_state();
//         if (policy.find(node) != policy.end())
//         {
//             found = true;
//         }
//         else
//         {
//             if(node.get_id() == -587737242){
//               cout << "here" << endl;
//             }
//             if(node.get_id() == -1610065062){
//               cout << "here" << endl;
//             }
//             vector<Operator> appli_ops;
//             set<Operator> deactivated_ops = node.get_deactivated_op();
//             for (std::set<PolicyNode>::iterator it = policy_node.begin(); it != policy_node.end(); ++it)
//             {
//                 ResilientNodeFormula current_node_formula = (*it).get_resilient_node_formula();
//                 Operator possible_app_op = (*it).get_operator();
//                 set<Operator> forbid = current_node_formula.get_deactivated_op();
//                 if(current_node_formula.get_formula().is_implied(state) && current_node_formula.get_k() == node.get_k() && current_node_formula.get_deactivated_op() == node.get_deactivated_op())
//                   	appli_ops.push_back(possible_app_op);
//             }
//             for (int i = 0; i < appli_ops.size(); i++){
//                 if(macro_to_op.find(appli_ops[i].get_name()) != macro_to_op.end()){
//                     appli_ops[i] = macro_to_op[appli_ops[i].get_name()];
//                 }
//                 if (deactivated_ops.find(appli_ops[i]) != deactivated_ops.end()){
//                     appli_ops.erase(appli_ops.begin() + i);
//                     i --;
//                     }
//             }
//             if(appli_ops.size() == 0){
//             	for (std::set<PolicyNode>::iterator it = policy_node.begin(); it != policy_node.end(); ++it)
//             	{
//                		ResilientNodeFormula current_node_formula = (*it).get_resilient_node_formula();
//                 	Operator possible_app_op = (*it).get_operator();
//                     set<Operator> forbid = node.get_deactivated_op();
//                 		if(current_node_formula.get_formula().is_implied(state))
//                           if(!(possible_app_op.get_name().find("macro_") != std::string::npos))
//                     		if (forbid.find(possible_app_op) == forbid.end())
//                           		if(possible_app_op.is_applicable(node.get_state()))
//                   					appli_ops.push_back(possible_app_op);

//             	}
//             }
//             // eliminare correttamente le azioni che cosi si sballa l indice

//             if(node.get_id() == 1539772529){
//               cout << appli_ops[0].get_name() << endl;
//             }
//             for (size_t i = 0; i < appli_ops.size(); ++i) {
//                 bool current_level = false;
//                 bool lower_level = false;
//                 Operator app_op = appli_ops[i];
//                 if(app_op.is_applicable(node.get_state())){
//                     State successor = registry->get_successor_state(node.get_state(), app_op);
//                     ResilientNode successor_node = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>
//                     PartialState successor_partial = (PartialState)successor;
//                     if(node.get_k() == 0 || app_op.is_safe()){
//                         for (std::tr1::unordered_map<int, ResilientNodeFormula>::iterator it = resilient_nodes_formula.begin();  it != resilient_nodes_formula.end(); ++it){
//                                 ResilientNodeFormula res = it->second;
//                                 PartialState formula = res.get_formula();
//                                 if ((formula.is_implied(successor_partial) && node.get_k() == res.get_k()) || goal.is_implied(successor_partial))
//                                 {
//                                     found = true;
//                                     policy[node] = app_op;
//                                     if (!goal.is_implied(successor_partial)){
//                                         open.push_back(successor_node);
//                                     }
//                                     break;
//                                 }
//                            }
//                            if(found){
//                              break;
//                              }
//                     }else{
//                         std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
//                         forbidden_plus_current.insert(app_op);
//                         ResilientNode current_without_action = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); // <s, k-1, V / a>
//                         for (std::tr1::unordered_map<int, ResilientNodeFormula>::iterator it = resilient_nodes_formula.begin();  it != resilient_nodes_formula.end(); ++it){
//                             ResilientNodeFormula res = it->second;
//                             PartialState formula = res.get_formula();
//                             if(node.get_id() == -1610065062){
//                             if(formula.is_implied(successor_partial)){
//                                	current_level = true;
//                            	}
//                             if(formula.is_implied(state)){
//                            	    lower_level = true;
//                            	}
//                             if(current_level && lower_level)
//                               cout << "here"<< endl;
//                             }
//                             if(formula.is_implied(successor_partial)){
//                                	current_level = true;
//                            	}
//                            	if(formula.is_implied(state) &&  res.get_k() == (node.get_k() - 1) && res.get_deactivated_op() == forbidden_plus_current){
//                            	    lower_level = true;
//                            	}
//                             if (current_level && lower_level){
//                              	if(current_without_action.get_id() == -1610065062 || successor_node.get_id() == -1610065062){
//                                   cout << "here" << endl;
//                              	}
//                                 found = true;
//                                 policy[node] = app_op;
//                                 open.push_back(current_without_action);
//                                 if (!goal.is_implied(successor_partial))
//                                     open.push_back(successor_node);
//                                 break;
//                             }
//                         }
//                         if(found){
//                           break;
//                         }
//                     }
//                 }
//             }
//         }
//         i++;

//         if (!found)
//         {
//             cout << "\nNode not found at iteration #" << i << endl;
//             node.dump();
//             cout << "next_actions:" << endl;
//             // for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
//             //     cout << it_o->get_name() << endl;
//             not_found_counter += 1;
//         }
//     }

//     if (not_found_counter > 0)
//         cout << "\n\nResilient policy created, but " << not_found_counter << " nodes not found." << endl;
//     else
//         cout << "\n\nResilient policy created, " << policy.size() << " nodes found." << endl;
// }
