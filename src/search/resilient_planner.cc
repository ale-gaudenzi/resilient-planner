/** @file */

#include "globals.h"
#include "operator.h"
#include "option_parser.h"
#include "ext/tree_util.hh"
#include "timer.h"
#include "utilities.h"
#include "search_engine.h"
#include "policy-repair/regression.h"
#include "policy-repair/policy.h"
#include "policy-repair/partial_state.h"
#include "resilient_node.h"
#include "search_engine.h"

#include <iostream>
#include <list>
#include <new>
#include <string>
#include <vector>
#include <stack>
#include <tr1/functional>
#include <sys/resource.h>

using namespace std;

bool resiliency_check(ResilientNode node);
bool replan(ResilientNode current_node, SearchEngine *engine);
void reset_goal();
void add_fault_model_deadend(ResilientNode node);
void print_results();
void print_timings();
void print_policy_res();
bool find_in_nodes_list(std::list<ResilientNode> set, ResilientNode node);
bool find_in_op_set(std::set<Operator> set, Operator op);
void resource_usage(string o);

std::list<ResilientNode> resilient_nodes;
std::stack<ResilientNode> nodes;

int main(int argc, const char **argv)
{
    register_event_handlers();

    if (argc < 2)
    {
        cout << OptionParser::usage(argv[0]) << endl;
        exit_with(EXIT_INPUT_ERROR);
    }

    if (string(argv[1]).compare("--help") != 0)
        read_everything(cin);

    SearchEngine *engine = 0;
    g_policy = 0;

    g_timer_regression.stop();
    g_timer_engine_init.stop();
    g_timer_search.stop();
    g_timer_policy_build.stop();
    g_timer_policy_use.stop();

    g_timer_regression.reset();
    g_timer_engine_init.reset();
    g_timer_search.reset();
    g_timer_policy_build.reset();
    g_timer_policy_use.reset();

    // the input will be parsed twice:
    // once in dry-run mode, to check for simple input errors,
    // then in normal mode
    g_timer_engine_init.resume();
    try
    {
        OptionParser::parse_cmd_line(argc, argv, true);
        engine = OptionParser::parse_cmd_line(argc, argv, false);
    }
    catch (ParseError &pe)
    {
        cerr << pe << endl;
        exit_with(EXIT_INPUT_ERROR);
    }
    g_timer_engine_init.stop();

    /* HAZ: Unfortunately, this must go here (as supposed to globals.cc)
     *      since we need to know if g_detect_deadends is true or not. */
    if (g_detect_deadends)
    {
        generate_regressable_ops();
    }
    /* HAZ: We create the policies even if we aren't using deadends, as
     *      they may be consulted by certain parts of the code. */
    g_deadend_policy = new Policy();
    g_deadend_states = new Policy();
    g_temporary_deadends = new Policy();

    /***************************************
     * Assert the settings are consistent. *
     ***************************************/
    if (((g_record_online_deadends || g_generalize_deadends) && !g_detect_deadends) ||
        ((g_partial_planlocal || g_plan_locally_limited) && !g_plan_locally) ||
        (g_optimized_scd && (g_jic_limit == 0)) ||
        (g_forgetpolicy && (g_jic_limit > 0)))
    {
        cout << "\n  Parameter Error: Make sure that the set of parameters is consistent.\n"
             << endl;
        exit(0);
    }

    /*********************
     * Handle JIC Limits *
     *********************/
    cout << "\nTotal allotted time (s): " << g_jic_limit << endl;

    // If we are going to do a final FSAP-free round, then we modify the
    //  time limits to give a 50/50 split between the JIC phase and final
    //  round phase
    double jic_ratio = 0.5;
    if (g_final_fsap_free_round)
        g_jic_limit *= jic_ratio;

    cout << "Max time for core JIC (remaining used in final-round repairs): " << g_jic_limit << endl;

    // Adjust the g_jic_limit so the epochs are handled properly
    int epochs_remaining = g_num_epochs;
    double single_jic_limit = g_jic_limit / (double)g_num_epochs;
    g_jic_limit = single_jic_limit;

    cout << "Max time for each of the " << epochs_remaining << " epochs: " << g_jic_limit << endl
         << endl;

    /************************
     * Initial policy search *
     *************************/
    engine->reset();
    g_timer_search.resume();
    engine->search();
    g_timer_search.stop();
    engine->save_plan_if_necessary();
    engine->statistics();
    engine->heuristic_statistics();
    cout << "Initial search time: " << g_timer_search << endl;
    cout << "Initial total time: " << g_timer << endl;

    if (!engine->found_solution())
    {
        cout << "No solution -- aborting repairs." << endl;
        exit(1);
    }

    cout << "\nRegressing the plan..." << endl;
    list<PolicyItem *> regression_steps = perform_regression(engine->get_plan(), g_matched_policy, 0, true);

    cout << "\nGenerating an initial policy..." << endl;
    g_policy = new Policy();
    g_policy->update_policy(regression_steps);

    if (g_sample_for_depth1_deadends)
        sample_for_depth1_deadends(engine->get_plan(), new PartialState(g_initial_state()));

    /***********************
     * Resilient Alghoritm *
     ***********************/
    // the resilient algorithm start from here, previously code is cloned from the original prp
    // save the first policy as the original policy
    // g_policy during the execution will contain all the other branches too
    g_original_policy = new Policy();
    g_original_policy->update_policy(regression_steps);

    // first filling of the nodes stack with the initial policy
    State current = g_initial_state();
    std::vector<const Operator *> plan = engine->get_plan();
    ResilientNode initial_node = ResilientNode(current, g_max_faults);
    g_current_faults = g_max_faults;

    for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
    {
        ResilientNode res_node = ResilientNode(current, g_max_faults);

        std::set<Operator> post_actions;
        post_actions.insert(*(*it));
        ResilientNode res_node_f = ResilientNode(current, g_max_faults - 1, post_actions);
/*
        if (g_verbose)
        {
            cout << "\nPushing nodes of the initial policy:" << endl;
            res_node.dump();
            res_node_f.dump();
        }
*/
        nodes.push(res_node);
        nodes.push(res_node_f);

        current = g_state_registry->get_successor_state(current, *(*it));
    }

    //cout << "\nInitial policy:" << endl;
    //g_policy->dump();

    // main while loop of the algorithm, basically 1:1 with the pseudocode
    int iteration = 1;
    while (!nodes.empty())
    {
        ResilientNode current_node = nodes.top();
        nodes.pop();
        g_current_faults = current_node.get_k();
        g_current_forbidden_ops = current_node.get_deactivated_op();

        k_v_pair current_pair = std::make_pair(g_current_faults, g_current_forbidden_ops);


        if (g_verbose)
        {
            cout << "\n----------------------------------------" << endl;
            cout << "\nIteration:" << iteration << endl;
            cout << "Current node:" << endl;
            current_node.dump();
        }

        if (!resiliency_check(current_node))
        {
            if (g_verbose)
                cout << "\nFailed resiliency check.\n"
                     << endl;

            for (int i = 0; i < g_operators.size(); i++)
            {
                if(g_current_forbidden_ops.find(g_operators[i]) != g_current_forbidden_ops.end()) {
                    g_operators.erase(g_operators.begin()+i);
                    i--;
                }
            }

            if (!replan(current_node, engine))
            {
                for (set<Operator>::iterator it = g_current_forbidden_ops.begin(); it != g_current_forbidden_ops.end(); ++it)
                {
                    g_operators.push_back(*it);
                }   

                if (g_verbose)
                    cout << "\nFailed replanning." << endl;
                add_fault_model_deadend(current_node);
            }
            else
            {
                for (set<Operator>::iterator it = g_current_forbidden_ops.begin(); it != g_current_forbidden_ops.end(); ++it)
                {
                    g_operators.push_back(*it);
                }   

                if (g_verbose)
                    cout << "Successfull replanning" << endl;

                State current = g_initial_state();
                std::vector<const Operator *> plan = engine->get_plan();

                if (current_node.get_k() >= 1)
                {
                    //if (g_verbose)
                    //    cout << "\nPushing nodes:\n"
                    //         << endl;

                    for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                    {
                        ResilientNode res_node = ResilientNode(current, g_current_faults, g_current_forbidden_ops);

                        std::set<Operator> post_actions = g_current_forbidden_ops;
                        post_actions.insert(*(*it));
                        ResilientNode res_node_f = ResilientNode(current, g_current_faults - 1, post_actions);
                        /*
                        if (g_verbose)
                        {
                            res_node.dump();
                            res_node_f.dump();
                            cout << "\n"
                                 << endl;
                        }*/

                        nodes.push(res_node);
                        nodes.push(res_node_f);
                        current = g_state_registry->get_successor_state(current, *(*it));
                    }
                }
                else
                {
                    for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                    {
                        ResilientNode res_node = ResilientNode(current, 0, current_node.get_deactivated_op());
                        /*if (g_verbose)
                        {
                            cout << "Pushing to R" << endl;
                            res_node.dump();
                        }*/
                        resilient_nodes.push_back(res_node);
                        current = g_state_registry->get_successor_state(current, *(*it));
                    }
                }

                regression_steps.clear();
                regression_steps = perform_regression(engine->get_plan(), g_matched_policy, 0, true);

                Policy *resilient_policy = new Policy();
                resilient_policy->update_policy(regression_steps);
                g_resilient_policies.insert(std::make_pair(std::make_pair(g_current_faults, g_current_forbidden_ops), resilient_policy));

                g_policy->update_policy(regression_steps);
            } 
        }
        else
        {

            if (g_verbose)
                cout << "\nSuccessfull resiliency check.\n"
                     << endl;
        }

        iteration++;
        //if (iteration > 2)
        //    break;
    }

    if (find_in_nodes_list(resilient_nodes, initial_node))
    {
        print_results();
    }
    else
    {
        cout << "\nInitial state is a deadend, problem is not " << g_max_faults << "-resilient!\n"
             << endl;
    }

    if (1 == g_dump_policy)
    {
        cout << "Dumping the policy and fsaps..." << endl;
        ofstream outfile;

        outfile.open("policy.out", ios::out);
        g_policy->generate_cpp_input(outfile);
        outfile.close();

        outfile.open("policy.fsap", ios::out);
        g_deadend_policy->generate_cpp_input(outfile);
        outfile.close();
    }
    else if (2 == g_dump_policy)
    {
        cout << "Dumping the policy and fsaps..." << endl;
        g_policy->dump_human_policy();
        g_deadend_policy->dump_human_policy(true);
    }

    g_timer.stop();
    print_timings();
}

/// Checks if the given node is resilient, using the current global policy to find the applicable next actions
/// then the StateRegistry linked to the state of the node to find the successors of the node.
/// The check is the same as the one in the pseudocode: (⟨s[a], k, V ⟩ ∈ R ∨ s[a] |= G) ∧ ⟨s, k − 1, V ∪ {a}⟩ ∈ R.
/// It also add the node to resilient_nodes list if the check succeds.
/// @param node The node to check if is resilient or not.
/// @return True if the node is resilient, false otherwise.
bool resiliency_check(ResilientNode node)
{
    if (resilient_nodes.empty())
        return false;

    PartialState state_to_check = PartialState(node.get_state());

    if (find_in_nodes_list(resilient_nodes, node))
        return true;

    std::set<Operator> next_actions;
    next_actions.clear();
    list<PolicyItem *> current_policy = g_policy->get_items();
    PolicyItem *goal_step = NULL;

    // implementation of getPolicyActions(P,s) in the pseudocode
    for (std::list<PolicyItem *>::iterator it = current_policy.begin(); it != current_policy.end(); ++it)
    {
        RegressionStep *reg_step = dynamic_cast<RegressionStep *>(*it);
        if (!reg_step->is_goal)
        {
            PartialState policy_state = PartialState(*reg_step->state);

            if ((*reg_step->state).implies(state_to_check) && !find_in_op_set(node.get_deactivated_op(), reg_step->get_op()))
                next_actions.insert(reg_step->get_op());
        }
        else if (reg_step->is_goal)
            goal_step = reg_step;

    }

    State state = node.get_state();
    StateRegistry *registry = const_cast<StateRegistry *>(&state.get_registry());

    for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
    {
        State successor = registry->get_successor_state(node.get_state(), *it_o);
        PartialState successor_p = PartialState(successor);
        ResilientNode successor_r = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>

        std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
        forbidden_plus_current.insert(*it_o);
        ResilientNode current_r = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); // <s, k-1, V U {a}>

        if ((find_in_nodes_list(resilient_nodes, successor_r) || (*goal_step->state).implies(successor_p)) && (find_in_nodes_list(resilient_nodes, current_r)))
        {
            resilient_nodes.push_back(node);
            return true;
        }
    }
    return false;
}

/// @brief Try to replan from the state contained in current_node to the goal.
/// The search engine is resetted and not recreated to avoid the overhead of the initialization.
/// The plan is saved inside the engine object and can be retrieved from it.
/// @param current_node The node containing the state to replan from.
/// @param engine The search engine originally created.
/// @return True if the replan succeds, false otherwise.
bool replan(ResilientNode current_node, SearchEngine *engine)
{
    if (g_verbose)
    {
        cout << "Replanning... " << endl;
        resource_usage("Before replan");
    }

    PartialState current_state = PartialState(current_node.get_state());

    if (is_deadend(current_state))
    {
        if (g_verbose)
            cout << "\nDetected deadend" << endl;
        return false;
    }

    if (g_verbose)
        cout << "\nCreating initial state." << endl;

    // set the initial state as the state in the current node
    g_state_registry->reset_initial_state();
    for (int i = 0; i < g_variable_name.size(); i++)
        g_initial_state_data[i] = current_state[i];

    if (g_verbose)
        cout << "Creating new engine." << endl;

    reset_goal();
    g_timer_engine_init.resume();
    engine->reset();
    g_timer_engine_init.stop();

    if (g_verbose)
        cout << "Searching for a solution." << endl;

    g_timer_search.resume();
    engine->search();
    g_timer_search.stop();

    if (engine->found_solution())
    {
        cout << "Solution found" << endl;
        if (g_verbose)
        {
            engine->save_plan_if_necessary();
            engine->statistics();
            engine->heuristic_statistics();
            resource_usage("After replan");
        }

        return true;
    }
    else
    {
        if (g_verbose)
            cout << "Replanning failed!" << endl;

        return false;
    }
}

/// @brief Regress the state contained in node and add every state-action pair Regr(s,A) 
/// to the fault model policy map indexed by the current (k,V).
/// @param node Node containing the state to regress and the current (k,V).
void add_fault_model_deadend(ResilientNode node)
{
    State state = node.get_state();
    list<PolicyItem *> de_items;

    PartialState *dummy_state = new PartialState();

    PartialState *de_state = new PartialState(state);
    generalize_deadend(*de_state);

    vector<PolicyItem *> reg_items;
    g_regressable_ops->generate_applicable_items(*de_state, reg_items, true, g_regress_only_relevant_deadends);

    for (int j = 0; j < reg_items.size(); j++)
    {
        RegressableOperator *ro = (RegressableOperator *)(reg_items[j]);
        de_items.push_back(new NondetDeadend(new PartialState(*de_state, *(ro->op), false, dummy_state),
                                             ro->op->nondet_index));
    }

    delete dummy_state;
    Policy *current_deadend_policy = new Policy();

    current_deadend_policy->update_policy(de_items);
    g_fault_models.insert(std::make_pair(std::make_pair(g_current_faults, g_current_forbidden_ops), current_deadend_policy));

    std::set<Operator> v = node.get_deactivated_op();
    for (std::set<Operator>::iterator it = v.begin(); it != v.end(); ++it)
    {
        std::set<Operator> forbidden_minus_a = g_current_forbidden_ops;
        forbidden_minus_a.erase(*it);

        Policy *s_a = new Policy();
        list<PolicyItem *> s_a_item;

        s_a_item.push_back(new NondetDeadend(new PartialState(state), it->nondet_index));

        if (g_fault_models.find(std::make_pair(g_current_faults + 1, forbidden_minus_a)) != g_fault_models.end())
        {
            g_fault_models.find(std::make_pair(g_current_faults + 1, forbidden_minus_a))->second->update_policy(s_a_item);
        }
        else
        {
            s_a->update_policy(s_a_item);
            g_fault_models.insert(std::make_pair(std::make_pair(g_current_faults + 1, forbidden_minus_a), s_a));
        }
    }
}

/// @brief Reset the goal, used before replanning. Not sure if is really useful, but it was present in prp policy repair.
void reset_goal()
{
    g_goal.clear();
    for (int i = 0; i < g_goal_orig.size(); i++)
        g_goal.push_back(make_pair(g_goal_orig[i].first, g_goal_orig[i].second));
}

/// @brief Check if the node is present in the resilient nodes list, using the custom minority and equality operators.
bool find_in_nodes_list(std::list<ResilientNode> res_set, ResilientNode node)
{
    if (res_set.size() == 0)
    {
        return false;
    }
    else
    {
        bool found;
        for (std::list<ResilientNode>::iterator it = res_set.begin(); it != res_set.end(); ++it)
        {

            found = true;
            for (int i = 0; i < g_variable_domain.size(); i++)
            {
                const string &fact_name1 = g_fact_names[i][(it->get_state())[i]];
                const string &fact_name2 = g_fact_names[i][(node.get_state())[i]];
                if (fact_name1 != "<none of those>" && fact_name2 != "<none of those>" && fact_name1.compare(fact_name2) != 0)
                {
                    found = false;
                }
            }

            if (it->get_k() != node.get_k())
            {
                found = false;
            }

            if (it->get_deactivated_op().size() != node.get_deactivated_op().size())
            {
                found = false;
            }

            std::set<Operator> prova = it->get_deactivated_op();
            for (std::set<Operator>::iterator it_o = prova.begin(); it_o != prova.end(); ++it_o)
            {
                bool equal_op = false;
                if (find_in_op_set(node.get_deactivated_op(), *it_o))
                    equal_op = true;
                if (!equal_op)
                {
                    found = false;
                }
            }
            if (found)
            {
                return true;
            }
        }
        return false;
    }
}

/// @brief Check if the operator is present in the operator set.
bool find_in_op_set(std::set<Operator> op_set, Operator op)
{
    if (op_set.empty())
        return false;

    for (std::set<Operator>::iterator it = op_set.begin(); it != op_set.end(); ++it)
    {
        if (it->get_nondet_name() == op.get_nondet_name())
        {
            return true;
        }
    }
    return false;
}

/// @brief Print first base policy found and other branches generated from replanning
void print_results()
{
    cout << "\n\n--------------------------------------------------------------------" << endl;
    cout << "\n                  -{ Base policy without faults}-\n"
         << endl;
    cout << "--------------------------------------------------------------------\n"
         << endl;

    g_original_policy->dump_simple();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "\n                  -{ Alternative policies }-\n"
         << endl;
    cout << "--------------------------------------------------------------------" << endl;

    int i = 1;
    for (std::map<k_v_pair, Policy *>::iterator it = g_resilient_policies.begin(); it != g_resilient_policies.end(); ++it)
    {
        cout << "\n-{ Branch number " << i << " }-\n"
             << endl;
        cout << "Remaining faults : " << it->first.first << endl;
        cout << "\nFailed operator : " << endl;
        for (std::set<Operator>::iterator it_o = it->first.second.begin(); it_o != it->first.second.end(); ++it_o)
            cout << it_o->get_nondet_name() << endl;
        it->second->dump_simple();
        i++;
        cout << "\n--------------------------------------------------------------" << endl;
    }
    print_policy_res();
}

/// @brief Print time statistics
void print_timings()
{
    cout << "\n                  -{ Timing Statistics }-\n"
         << endl;
    cout << "         Engine Initialization: " << g_timer_engine_init << endl;
    cout << "                   Search Time: " << g_timer_search << endl;
    cout << "           Policy Construction: " << g_timer_policy_build << endl;
    cout << "                    Total time: " << g_timer << endl;
    cout << "\n--------------------------------------------------------------------\n"
         << endl;
}

void print_policy_res() 
{
    cout << "\nGlobal policy:\n\n";
    g_policy->dump_state_op();
    cout << "\n--------------------------------------------------------------------\n" << endl;
    cout << "Resilient nodes:\n\n";
    for (std::list<ResilientNode>::iterator it = resilient_nodes.begin(); it != resilient_nodes.end(); ++it)
    {
        it->dump();
        cout << endl;
    }
    cout << "\n--------------------------------------------------------------------\n" << endl;

}

/// @brief Print memory usage in a particural moment
void resource_usage(string o = "")
{
    int who = RUSAGE_SELF;
    struct rusage usage;
    getrusage(who, &usage);
    cout << "\n MEM USAGE " << o << " " << usage.ru_maxrss << " " << usage.ru_idrss << " " << usage.ru_isrss << "\n";
}