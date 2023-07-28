#include "print_utils.h"

#include "globals.h"
#include "ext/json.h"

#include <vector>
#include <list>
#include <map>
#include <tr1/functional>

using namespace std;
using namespace json;

/// @brief Print all the plans generate by replan function
void print_branches()
{
    jobject dump = jobject();
    vector<jobject> items;

    int i = 1;
    for (std::map<k_v_pair, Policy *>::iterator it = g_resilient_policies.begin(); it != g_resilient_policies.end(); ++it)
    {
        jobject item = jobject();
        item["id"] = ++i;

        vector<string> state;
        PartialState s = it->second->get_initial_state();
        int *vars = s.get_vars();
        for (int i = 0; i < g_variable_domain.size(); i++)
        {
            if (-1 != vars[i])
            {
                const string &fact_name = g_fact_names[i][vars[i]];
                if (fact_name != "<none of those>")
                    state.push_back(fact_name);
            }
        }
        item["state"] = state;

        item["k"] = it->first.first;

        vector<string> forbidden_op_names;
        for (std::set<Operator>::iterator it_o = it->first.second.begin(); it_o != it->first.second.end(); ++it_o)
            forbidden_op_names.push_back(it_o->get_nondet_name());
        item["forbidden"] = forbidden_op_names;

        vector<string> plan_op_names;
        list<PolicyItem *> policy = it->second->get_items();
        for (list<PolicyItem *>::reverse_iterator op_iter = policy.rbegin();
             op_iter != policy.rend(); ++op_iter)
        {
            RegressionStep *rs = (RegressionStep *)(*op_iter);
            if (!rs->is_goal)
                plan_op_names.push_back(rs->op->get_nondet_name());
        }
        item["plan"] = plan_op_names;

        items.push_back(item);
    }

    dump["items"] = items;
    std::string serial = (std::string)dump["items"];
    ofstream policy_file;
    policy_file.open("branches.json");
    policy_file << serial;
}

/// @brief Print statistics
void print_statistics(int resilient_nodes_size, int non_resilient_nodes_size)
{
    cout << "\n\n--------------------------------------------------------------------" << endl;
    cout << "\n                      -{ Statistics }-\n\n";
    cout << "                    Iterations: " << g_iteration << endl;
    cout << "   Successful resiliency check: " << g_successful_resiliency_check << endl;
    cout << "             Successful replan: " << g_successful_replan << endl;
    cout << "                      Deadends: " << g_deadend_states->get_size() << endl;
    cout << "               Resilient nodes: " << resilient_nodes_size << endl;
    cout << "           Non-resilient nodes: " << non_resilient_nodes_size << endl;
    cout << "               Actions planned: " << g_policy->get_size() << endl;
    cout << "      Max dimension open stack: " << g_max_dimension_open << endl;
    cout << "\n--------------------------------------------------------------------\n";
}

/// @brief Print time statistics
void print_timings()
{
    cout << "\n                  -{ Timing Statistics }-\n\n";
    cout << "         Engine Initialization: " << g_timer_engine_init << endl;
    cout << "                   Search Time: " << g_timer_search << endl;
    cout << "           Policy Construction: " << g_timer_policy_build << endl;
    cout << "                    Main cycle: " << g_timer_cycle << endl;
    cout << "     Resilient plan extraction: " << g_timer_extraction << endl;
    cout << "   Resilient policy extraction: " << g_timer_extract_policy << endl;
    cout << "                    Total time: " << g_timer << endl;
    cout << "\n--------------------------------------------------------------------\n";
}

/// @brief Print memory statistics
void print_memory()
{
    cout << "\n                  -{ Memory Statistics }-\n\n";
    cout << "                         Start: " << g_mem_initial << " KB" << endl;
    cout << "                     Algorithm: " << g_mem_post_alg << " KB" << endl;
    cout << "             Policy extraction: " << g_mem_extraction << " KB" << endl;
    cout << "                          Peak: " << get_peak_memory_in_kb() << " KB" << endl;
    cout << "\n--------------------------------------------------------------------\n\n";
}

/// @brief Print the global policy
void print_policy()
{
    cout << "\n                  -{ Global policy }-\n\n";
    g_policy->dump_state_op();
    cout << "\n--------------------------------------------------------------------\n\n";
}

/// @brief Print all resilient nodes
void print_resilient_nodes(tr1::unordered_map<int, ResilientNode> resilient_nodes)
{
    std::ofstream out("resilient_nodes");
    std::streambuf *coutbuf = std::cout.rdbuf(); // save old buf
    std::cout.rdbuf(out.rdbuf());                // redirect std::cout to out.txt!
    cout << "\n                  -{ Resilient nodes }-\n\n";
    for (std::tr1::unordered_map<int, ResilientNode>::iterator it = resilient_nodes.begin(); it != resilient_nodes.end(); ++it)
    {
        it->second.dump();
        cout << endl;
    }
    cout << "\n--------------------------------------------------------------------\n\n";
    std::cout.rdbuf(coutbuf); // reset to standard output again
}

/// @brief Print all non resilient nodes
void print_non_resilient_nodes(tr1::unordered_map<int, ResilientNode> non_resilient_nodes)
{
    cout << "\n                -{ Non resilient nodes }-\n\n";
    for (std::tr1::unordered_map<int, ResilientNode>::iterator it = non_resilient_nodes.begin(); it != non_resilient_nodes.end(); ++it)
    {
        it->second.dump();
        cout << endl;
    }
    cout << "\n--------------------------------------------------------------------\n\n";
}

/// @brief Print a plan
/// @param to_file print to output or to file
/// @param plan the plan to print
void print_plan(bool to_file, std::list<Operator> plan)
{
    if (to_file)
    {
        ofstream plan_file;
        plan_file.open("resilient_plan");
        for (std::list<Operator>::iterator it = plan.begin(); it != plan.end(); ++it)
            plan_file << it->get_nondet_name() << endl;
    }
    else
    {
        cout << "\n\n--------------------------------------------------------------------\n";
        cout << "\n                  -{ Resilient plan }-\n\n";
        for (std::list<Operator>::iterator it = plan.begin(); it != plan.end(); ++it)
            cout << it->get_nondet_name() << endl;
    }
}

void print_resilient_policy(map<ResilientNode, Operator> policy)
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

void print_resilient_policy_json(map<ResilientNode, Operator> policy)
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