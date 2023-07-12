#include "resilient_policy.h"
#include "json.h"


using namespace json;
using namespace std;

Operator ResilientPolicy::get_successor(ResilientNode node)
{

    return policy.find(node)->second;
}

void ResilientPolicy::add_item(ResilientNode node, Operator op)
{
    if(policy.find(node) == policy.end())
        policy.insert(make_pair(node, op));
}

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
            if (resilient_nodes.find(successor_node.get_id()) != resilient_nodes.end() || ((PartialState)goal).is_implied(successor_p)) //|| ((PartialState)successor).is_implied(goal))
            {
                found = true;

                if (policy.find(successor_node) == policy.end())
                {
                    add_item(node, *it_o);

                    //if (!((PartialState)successor).is_implied(goal))
                    if(!goal.is_implied(successor_p))
                        open.push_back(successor_node);
                    
                    if (node.get_k() > 0)
                    {
                        std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
                        forbidden_plus_current.insert(*it_o);
                        ResilientNode current_r = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); // <s, k-1, V U {a}>
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
        cout << "Action:" << endl;
        cout << it->second.get_nondet_name() << endl;
    }

    cout.rdbuf(coutbuf);
}

void ResilientPolicy::dump_json()
{
    jobject dump = jobject();
    vector<jobject> items;
    int i = 0;

    for (std::map<ResilientNode, Operator>::iterator it = policy.begin(); it != policy.end(); ++it)
    {
        jobject item = jobject();
        item["#"] = ++i;
        item["id"] = it->first.get_id();

        vector<string> state;
        for (int j = 0; j < g_variable_domain.size(); j++)
        {
            const string &fact_name = g_fact_names[j][(it->first.get_state())[j]];
            if (fact_name != "<none of those>")
                state.push_back(fact_name);
        }
        item["state"] = state;

        item["k"] = it->first.get_k();

        set<Operator> deactivated_op = it->first.get_deactivated_op();
        vector<string> deactivated_op_names;
        for (std::set<Operator>::iterator it_o = deactivated_op.begin(); it_o != deactivated_op.end(); ++it_o)
            deactivated_op_names.push_back(it_o->get_nondet_name());
        item["forbidden"] = deactivated_op_names;

        item["action"] = it->second.get_nondet_name();

        items.push_back(item);
    }

    dump["items"] = items;

    std::string serial = (std::string)dump["items"];

    ofstream policy_file;
    policy_file.open("resilient_policy.json");
    policy_file << serial;
}
