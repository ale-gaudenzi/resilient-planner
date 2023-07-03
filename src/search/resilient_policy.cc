#include "resilient_policy.h"

using namespace std;


Operator ResilientPolicy::get_successor(ResilientNode node)
{

    return policy.find(node)->second;
}


void ResilientPolicy::add_item(ResilientNode node, Operator op)
{
    policy.insert(make_pair(node, op));
}


void ResilientPolicy::extract_policy(State initial_state, PartialState goal, int K, set<ResilientNode> resilient_nodes)
{
    stack<ResilientNode> open;
    open.push(ResilientNode(initial_state, K));

    while (!open.empty())
    {
        ResilientNode node = open.top();
        open.pop();

        list<PolicyItem *> current_policy = g_policy->get_items();
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

        for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
        {
            State successor = registry->get_successor_state(node.get_state(), *it_o);
            ResilientNode successor_node = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>
            
            if (find_in_nodes_set(resilient_nodes, successor_node) || ((PartialState)successor).is_implied(goal))
            {
                add_item(node, *it_o);

                if (!((PartialState)successor).is_implied(goal)) 
                    open.push(successor_node);
                
                if (node.get_k() > 0)
                {
                    std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
                    forbidden_plus_current.insert(*it_o);
                    ResilientNode current_r = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); // <s, k-1, V U {a}>
                    open.push(current_r);
                }
                break;
            }
        }
    }
}


void ResilientPolicy::dump()
{
    ofstream out("resilient_policy");
    streambuf *coutbuf = std::cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    int i = 0;
    for (std::map<ResilientNode, Operator>::iterator it = policy.begin(); it != policy.end(); ++it)
    {
        cout << "#" << i++ << endl;
        it->first.dump();
        cout << "\nAction:" << endl;
        it->second.dump();
        cout << endl;
    }

    cout.rdbuf(coutbuf);
}



