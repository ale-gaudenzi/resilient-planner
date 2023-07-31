#include "resilient_policy.h"
#include "ext/json.h"

using namespace json;
using namespace std;

/// Return the applicable next operator of the given node.
/// @param node The node to find the successor operator to.
/// @return The applicable operator.
Operator ResilientPolicy::get_successor(ResilientNode node)
{
    return policy.find(node)->second;
}

/// Add an item to the policy, checking if it is already present.
/// @param node the node to add to the policy.
/// @param op the operator corresponding to the node.
void ResilientPolicy::add_item(ResilientNode node, Operator op)
{
    if(policy.find(node) == policy.end())
        policy.insert(make_pair(node, op));
}

/// Extract the resilient policy from the global policy.
/// @param initial_state The first state of the policy.
/// @param goal The last state of the policy.
/// @param K The resilience parameter.
/// @param resilient_nodes The set of resilient nodes.
void ResilientPolicy::extract_policy(State initial_state, PartialState goal, int K, std::tr1::unordered_map<int, ResilientNode> resilient_nodes)
{
    list<ResilientNode> open;
    open.push_back(ResilientNode(initial_state, K, set<Operator>()));
    int i = 0;
    int counter = 0;
    list<PolicyItem *> current_policy = g_policy->get_items();
    
    cout << "\n\n--------------------------------------------------------------------\n\n";

    while (!open.empty())
    {
        ResilientNode node = open.front();
        open.pop_front();

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

        StateRegistry *registry = const_cast<StateRegistry *>(&(node.get_state()).get_registry());
        bool found = false;

        for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
        {
            State successor = registry->get_successor_state(node.get_state(), *it_o);
            ResilientNode successor_node = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>
            PartialState successor_p = (PartialState)successor;
            if (resilient_nodes.find(successor_node.get_id()) != resilient_nodes.end() || ((PartialState)goal).is_implied(successor_p)) 
            {
                found = true;

                if (policy.find(successor_node) == policy.end())
                {
                    add_item(node, *it_o);

                    if(!goal.is_implied(successor_p))
                        open.push_back(successor_node);
                    
                    if (node.get_k() > 0)
                    {
                        std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
                        forbidden_plus_current.insert(*it_o);
                        ResilientNode current_r = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); 
                        open.push_back(current_r);
                    }
                }
                break;
            }
        }

        if (!found)
        {
            cout << "\nNode not found at iteration #" << i << endl;
            node.dump();
            cout << "next_actions:" << endl;
            for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
                cout << it_o->get_name() << endl;
            
            counter += 1;
        }
   
        i++;
    }

    if (counter > 0)
        cout << "\nResilient policy created, but " << counter << " nodes not found."  << endl;    
    else
        cout << "\nResilient policy created, all nodes found." << endl;
}


