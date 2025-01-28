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

    list<std::pair<int, ResilientNode> > open;
    open.push_back(make_pair(-1, ResilientNode(initial_state, K, set<Operator>())));
    list<PolicyItem *> current_policy = g_policy->get_items();

    while (!open.empty())
    {
        int initial_state_dist;
        bool found = false;

        ResilientNode node = open.front().second;
        std::vector<int> node_distances;
        int dist = open.front().first;
        open.pop_front();
        StateRegistry *registry = const_cast<StateRegistry *>(&(node.get_state()).get_registry());
        PartialState state = (PartialState)node.get_state();

        std::map<int, vector<Operator> > dist_next_actions;
        for (std::list<PolicyItem *>::iterator it = current_policy.begin(); it != current_policy.end(); ++it)
        {
            RegressionStep *reg_step = dynamic_cast<RegressionStep *>(*it);

            if (!reg_step->is_goal)
            {
                PartialState policy_state = PartialState(*reg_step->state);
                if (policy_state.is_implied(state) && !find_in_op_set(node.get_deactivated_op(), reg_step->get_op())){
                    if (dist_next_actions.find(reg_step->get_distance()) != dist_next_actions.end()) {
                        dist_next_actions[reg_step->get_distance()].push_back(reg_step->get_op());
                    } else {
                        initial_state_dist = reg_step->get_distance();
                        vector<Operator> applicable_ops;
                        applicable_ops.push_back(reg_step->get_op());
                        dist_next_actions[reg_step->get_distance()] =  applicable_ops;
                    }
                }
            }
        }           
        if (policy.find(node) != policy.end())
        {
            found = true;
        }
        else
        {
            vector<Operator> applicable_ops;
            int possible_dist;
            if (dist == -1){
                applicable_ops = dist_next_actions[initial_state_dist];
                possible_dist = initial_state_dist;
            }
            else
            {
                if(dist_next_actions.size() == 1 || dist_next_actions.find(dist-1) == dist_next_actions.end()){
                    applicable_ops = dist_next_actions.begin()->second;
                    possible_dist = dist_next_actions.begin()->first;
                }else{
                    applicable_ops = dist_next_actions[dist - 1];
                    possible_dist = dist - 1;
                }
            }
            for (size_t i = 0; i < applicable_ops.size(); ++i) {
                Operator app_op = applicable_ops[i];
                if(relation_node_next_action.find(node.get_id()) != relation_node_next_action.end()){
                    app_op = relation_node_next_action[node.get_id()];
                }
                State successor = registry->get_successor_state(node.get_state(), app_op);
                ResilientNode successor_node = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>
                PartialState successor_partial = (PartialState)successor;

                if(node.get_k() == 0 || app_op.is_safe()){
                    if (resilient_nodes.find(successor_node.get_id()) != resilient_nodes.end() || goal.is_implied(successor_partial))
                    {
                        found = true;
                        policy[node] = app_op ;
                        if (!goal.is_implied(successor_partial)){
                            open.push_back(make_pair(possible_dist, successor_node));
                        }
                        break;
                    }
                }else{
                    std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
                    forbidden_plus_current.insert(app_op);
                    ResilientNode current_without_action = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); // <s, k-1, V / a>
                    if ((resilient_nodes.find(successor_node.get_id()) != resilient_nodes.end() && resilient_nodes.find(current_without_action.get_id()) != resilient_nodes.end()))
                    {
                        found = true;
                        policy[node] = app_op;
                        open.push_back(make_pair(dist, current_without_action));
                        if (!goal.is_implied(successor_partial))
                            open.push_back(make_pair(possible_dist, successor_node));
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
            // for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
            //     cout << it_o->get_name() << endl;
            not_found_counter += 1;
        }
    }

    if (not_found_counter > 0)
        cout << "\n\nResilient policy created, but " << not_found_counter << " nodes not found." << endl;
    else
        cout << "\n\nResilient policy created, " << policy.size() << " nodes found." << endl;
}
