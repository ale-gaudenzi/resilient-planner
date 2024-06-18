#include "resilient_policy.h"
#include "ext/json.h"

using namespace json;
using namespace std;

/// Extract the resilient policy from the global policy.
/// @param initial_state The first state of the policy.
/// @param goal The last state of the policy.
/// @param K The resilience parameter.
/// @param resilient_nodes The set of resilient nodes.
void ResilientPolicy::extract_policy(State initial_state, PartialState goal, int K, std::tr1::unordered_map<int, ResilientNode> resilient_nodes)
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
        open.pop_front();
        StateRegistry *registry = const_cast<StateRegistry *>(&(node.get_state()).get_registry());
        PartialState state = (PartialState)node.get_state();

        std::set<Operator> next_actions;
        for (std::list<PolicyItem *>::iterator it = current_policy.begin(); it != current_policy.end(); ++it)
        {
            RegressionStep *reg_step = dynamic_cast<RegressionStep *>(*it);

            if (!reg_step->is_goal)
            {
                PartialState policy_state = PartialState(*reg_step->state);

                if (policy_state.is_implied(state) && !find_in_op_set(node.get_deactivated_op(), reg_step->get_op()))
                    next_actions.insert(reg_step->get_op());
            }
        }

        if (policy.find(node) != policy.end())
        {
            found = true;
        }
        else
        {
            for (std::set<Operator>::reverse_iterator it_o = next_actions.rbegin(); it_o != next_actions.rend(); ++it_o)
            {
                State successor = registry->get_successor_state(node.get_state(), *it_o);
                ResilientNode successor_node = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>
                PartialState successor_partial = (PartialState)successor;

                if(node.get_k() == 0){
                    if (resilient_nodes.find(successor_node.get_id()) != resilient_nodes.end() || goal.is_implied(successor_partial))
                    {
                        std::map<ResilientNode, Operator>::iterator it = policy.find(successor_node);
                        if (!(it != policy.end()))
                        {
                            found = true;
                            policy.insert(make_pair(node, *it_o));
                            if (!goal.is_implied(successor_partial)){
                                open.push_back(successor_node);
                            }
                            break;
                        }
                    }
                }else{
                    std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
                    forbidden_plus_current.insert(*it_o);
                    ResilientNode current_without_action = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); // <s, k-1, V / a>

                    if ((resilient_nodes.find(successor_node.get_id()) != resilient_nodes.end() && resilient_nodes.find(current_without_action.get_id()) != resilient_nodes.end()))
                    {
                        found = true;
                        policy.insert(make_pair(node, *it_o));
                        open.push_back(current_without_action);
                        if (!goal.is_implied(successor_partial))
                            open.push_back(successor_node);
                        break;
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
            for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
                cout << it_o->get_name() << endl;
            not_found_counter += 1;
        }
    }

    if (not_found_counter > 0)
        cout << "\n\nResilient policy created, but " << not_found_counter << " nodes not found." << endl;
    else
        cout << "\n\nResilient policy created, " << policy.size() << " nodes found." << endl;
}