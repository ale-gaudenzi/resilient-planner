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
#include <fstream>
#include <list>
#include <new>
#include <string>
#include <vector>
#include <stack>
#include <tr1/functional>

#include <iostream>

using namespace std;

bool resiliency_check(ResilientNode node);
bool replan(ResilientNode current_node, SearchEngine *engine);
std::list<Operator> extract_solution();
void add_fault_model_deadend(ResilientNode node);
void update_non_resilient_nodes(ResilientNode node);
void print_branches();
void print_timings();
void print_policy();
void print_resilient_nodes();
void print_plan(bool to_file, std::list<Operator> plan);
void print_memory();
void print_statistics();

bool find_in_nodes_list(std::list<ResilientNode> set, ResilientNode node);
bool find_in_op_set(std::set<Operator> set, Operator op);
long mem_usage();

std::list<ResilientNode> resilient_nodes;
std::list<ResilientNode> non_resilient_nodes;
std::stack<ResilientNode> open;
int max_dimension_open = 0;
int replan_counter = 0;
int successful_replan = 0;
int successful_resiliency_check = 0;
int iteration = 0;

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
    g_policy = new Policy();
    list<PolicyItem *> regression_steps;

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
    g_timer_engine_init.resume();
    g_mem_initial = mem_usage();

    // the input will be parsed twice:
    // once in dry-run mode, to check for simple input errors,
    // then in normal mode
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

    generate_regressable_ops();

    /* HAZ: We create the policies even if we aren't using deadends, as
     *      they may be consulted by certain parts of the code. */
    g_deadend_policy = new Policy();
    g_deadend_states = new Policy();
    g_temporary_deadends = new Policy();

    if (((g_record_online_deadends || g_generalize_deadends) && !g_detect_deadends) ||
        ((g_partial_planlocal || g_plan_locally_limited) && !g_plan_locally) ||
        (g_optimized_scd && (g_jic_limit == 0)) ||
        (g_forgetpolicy && (g_jic_limit > 0)))
    {
        cout << "\n  Parameter Error: Make sure that the set of parameters is consistent.\n"
             << endl;
        exit(0);
    }

    cout << "Total allotted time (s): " << g_jic_limit << endl;

    g_operators_backup = g_operators;

    /***********************
     * Resilient Alghoritm *
     ***********************/

    ResilientNode initial_node = ResilientNode(g_initial_state(), g_max_faults);
    open.push(initial_node);

    while (!open.empty())
    {
        iteration++;
        if (open.size() > max_dimension_open)
            max_dimension_open = open.size();

        ResilientNode current_node = open.top();
        open.pop();
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

        if (!find_in_nodes_list(resilient_nodes, current_node) && !find_in_nodes_list(non_resilient_nodes, current_node))
        {
            if (resiliency_check(current_node))
            {
                successful_resiliency_check++;

                resilient_nodes.push_back(current_node);
                if (g_verbose)
                    cout << "\nSuccessfull resiliency check.\n"
                         << endl;
            }
            else
            {
                if (g_verbose)
                    cout << "\nFailed resiliency check.\n"
                         << endl;

                if (!replan(current_node, engine))
                {
                    if (g_verbose)
                        cout << "\nFailed replanning." << endl;
                    add_fault_model_deadend(current_node);
                    // non_resilient_nodes.push_back(current_node);
                    update_non_resilient_nodes(current_node);
                }
                else
                {
                    successful_replan++;
                    if (g_verbose)
                        cout << "Successfull replanning" << endl;

                    State current = g_initial_state();
                    std::vector<const Operator *> plan = engine->get_plan();
                    if (current_node.get_k() >= 1)
                    {
                        for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                        {
                            ResilientNode res_node = ResilientNode(current, g_current_faults, g_current_forbidden_ops);

                            std::set<Operator> post_actions = g_current_forbidden_ops;
                            post_actions.insert(*(*it));
                            ResilientNode res_node_f = ResilientNode(current, g_current_faults - 1, post_actions);

                            open.push(res_node);
                            open.push(res_node_f);
                            current = g_state_registry->get_successor_state(current, *(*it));
                        }
                        resilient_nodes.push_back(current_node);
                    }
                    else
                    {
                        for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                        {
                            ResilientNode res_node = ResilientNode(current, 0, current_node.get_deactivated_op());

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
        }

        if (g_max_iterations > 0 && iteration > g_max_iterations)
            break;
    }

    if (find_in_nodes_list(resilient_nodes, initial_node))
    {
        if (g_dump_branches)
            print_branches();
        if (g_dump_global_policy)
            print_policy();
        if (g_dump_resilient_nodes)
            print_resilient_nodes();

        print_plan(g_plan_to_file, extract_solution());
    }
    else
        cout << "\nInitial state is a deadend, problem is not " << g_max_faults << "-resilient!\n"
             << endl;

    g_mem_post_alg = mem_usage();

    print_statistics();

    g_timer.stop();
    print_timings();
    print_memory();
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

    std::set<Operator> next_actions;
    list<PolicyItem *> current_policy = g_policy->get_items();

    // find every action applicable in the current state, minus V
    for (std::list<PolicyItem *>::iterator it = current_policy.begin(); it != current_policy.end(); ++it)
    {
        RegressionStep *reg_step = dynamic_cast<RegressionStep *>(*it);
        if (!reg_step->is_goal)
        {
            PartialState policy_state = PartialState(*reg_step->state);

            if ((*reg_step->state).is_implied(state_to_check) && !find_in_op_set(node.get_deactivated_op(), reg_step->get_op()))
                next_actions.insert(reg_step->get_op());
        }
    }

    State state = node.get_state();
    StateRegistry *registry = const_cast<StateRegistry *>(&state.get_registry());

    // resiliency check cycle
    for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
    {
        State successor = registry->get_successor_state(node.get_state(), *it_o);
        PartialState successor_p = PartialState(successor);
        ResilientNode successor_r = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>

        std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
        forbidden_plus_current.insert(*it_o);
        ResilientNode current_r = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); // <s, k-1, V U {a}>

        if (find_in_nodes_list(resilient_nodes, successor_r) && find_in_nodes_list(resilient_nodes, current_r))
            return true;
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
    PartialState current_state = PartialState(current_node.get_state());

    if (is_deadend(current_state))
        return false;

    g_state_registry->reset_initial_state();
    for (int i = 0; i < g_variable_name.size(); i++)
        g_initial_state_data[i] = current_state[i];

    g_timer_engine_init.resume();
    engine->reset();
    g_timer_engine_init.stop();
    g_timer_search.resume();
    engine->search();
    g_timer_search.stop();

    if (g_dump_memory_replan_progression)
        cout << "Memory at replan #" << replan_counter + 1 << ": " << mem_usage() << "KB" << endl;

    return engine->found_solution();
}

/// @brief Extract the final resilient plan, starting by the initial state
/// and choosing only actions that lead to resilient states until the goal
/// is reached.
/// @return The plan extracted.
std::list<Operator> extract_solution()
{
    std::list<Operator> plan;
    State state = g_initial_state();
    PartialState partial_state = (PartialState)state;
    list<PolicyItem *> current_policy = g_policy->get_items();
    std::set<Operator> next_actions;
    PolicyItem *goal_step = current_policy.front();
    PartialState goal = PartialState(*goal_step->state);
    PartialState policy_state;

    // Consider the state registry of the initial state, since it is the same for all states
    // found by registry.successor() in sequence
    StateRegistry *registry = const_cast<StateRegistry *>(&state.get_registry());

    while (!goal.is_implied(partial_state))
    {
        for (std::list<PolicyItem *>::iterator it = current_policy.begin(); it != current_policy.end(); ++it)
        {
            RegressionStep *reg_step = dynamic_cast<RegressionStep *>(*it);
            policy_state = PartialState(*reg_step->state);

            if (!reg_step->is_goal && policy_state.is_implied(partial_state))
            {
                State successor = registry->get_successor_state(state, *reg_step->op);
                PartialState successor_p = (PartialState)successor;
                ResilientNode successor_node = ResilientNode(successor, g_max_faults);
                if (find_in_nodes_list(resilient_nodes, successor_node) || goal.is_implied(successor_p))
                {
                    plan.push_back(*reg_step->op);
                    state = successor;
                    partial_state = successor_p;
                    break;
                }
            }
        }
    }
    return plan;
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
            g_fault_models.find(std::make_pair(g_current_faults + 1, forbidden_minus_a))->second->update_policy(s_a_item);
        else
        {
            s_a->update_policy(s_a_item);
            g_fault_models.insert(std::make_pair(std::make_pair(g_current_faults + 1, forbidden_minus_a), s_a));
        }
    }
}

/// @brief Update non resilient nodes with every <s,k',V'> such that V' is a subset of V and k' = k - |V \ V'|.
/// @param node Deadend node to insert in the list non-resilient nodes.
void update_non_resilient_nodes(ResilientNode node)
{
    non_resilient_nodes.push_back(node);
    if (node.get_deactivated_op().size() != 0)
    {
        vector<std::set<Operator> > subsets;
        std::set<Operator> v_set = node.get_deactivated_op();
        // copy original set into vector to apply bitmasking method to find the power set
        vector<Operator> vec;

        for (std::set<Operator>::iterator it = v_set.begin(); it != v_set.end(); it++)
        {
            Operator to_push = *it;
            vec.push_back(to_push);
        }

        // bit masking method for finding subsets
        int set_size = node.get_deactivated_op().size();
        int pow_set_size = pow(2, set_size);
        int counter, j;

        // Run from counter 000..0 to 111..1
        for (counter = 0; counter < pow_set_size; counter++)
        {
            set<Operator> current;
            for (j = 0; j < set_size; j++)
            {
                // Check if jth bit in the counter is set
                // If set then print jth element from set
                if (counter & (1 << j))
                    current.insert(vec[j]);
            }
            subsets.push_back(current);
        }

        for (int i = 0; i < subsets.size() - 1; i++)
        {
            set<Operator> subset = subsets[i];
            int k1 = node.get_k() + (node.get_deactivated_op().size() - subset.size());
            ResilientNode to_add = ResilientNode(node.get_state(), k1, subset);
            to_add.dump();
            non_resilient_nodes.push_back(to_add);
        }
    }
    return;
}

/// @brief Check if the node is present in the resilient nodes list, using the custom minority and equality operators.
bool find_in_nodes_list(std::list<ResilientNode> res_set, ResilientNode node)
{
    if (res_set.size() == 0)
        return false;
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
                    found = false;
            }

            if (it->get_k() != node.get_k())
                found = false;

            if (it->get_deactivated_op().size() != node.get_deactivated_op().size())
                found = false;

            std::set<Operator> prova = it->get_deactivated_op();
            for (std::set<Operator>::iterator it_o = prova.begin(); it_o != prova.end(); ++it_o)
            {
                bool equal_op = false;
                if (find_in_op_set(node.get_deactivated_op(), *it_o))
                    equal_op = true;
                if (!equal_op)
                    found = false;
            }
            if (found)
                return true;
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
        if (it->get_nondet_name() == op.get_nondet_name())
            return true;

    return false;
}

/// @brief Print all the plans generate by replan function
void print_branches()
{
    ofstream plan_file;
    plan_file.open("branches");

    int i = 1;
    for (std::map<k_v_pair, Policy *>::iterator it = g_resilient_policies.begin(); it != g_resilient_policies.end(); ++it)
    {
        plan_file << "#" << i << endl;
        plan_file << "Remaining faults : \n"
                  << it->first.first << endl;
        plan_file << "Failed operators : " << endl;
        for (std::set<Operator>::iterator it_o = it->first.second.begin(); it_o != it->first.second.end(); ++it_o)
            plan_file << it_o->get_nondet_name() << endl;

        plan_file << "State : " << endl;
        PartialState s = it->second->get_initial_state();
        int *vars = s.get_vars();
        for (int i = 0; i < g_variable_domain.size(); i++)
        {
            if (-1 != vars[i])
            {
                const string &fact_name = g_fact_names[i][vars[i]];
                if (fact_name != "<none of those>")
                    plan_file << fact_name << endl;
                else
                    plan_file << "[" << g_variable_name[i] << "] None of those." << endl;
            }
        }

        plan_file << "Plan : " << endl;
        list<PolicyItem *> policy = it->second->get_items();
        for (list<PolicyItem *>::reverse_iterator op_iter = policy.rbegin();
             op_iter != policy.rend(); ++op_iter)
        {
            RegressionStep *rs = (RegressionStep *)(*op_iter);
            if (!rs->is_goal)
                plan_file << rs->op->get_nondet_name() << endl;
        }
        i++;
    }
    plan_file << "#" << endl;
}

/// @brief Print statistics
void print_statistics()
{
    cout << "\n                      -{ Statistics }-\n"
         << endl;
    cout << "                    Iterations: " << iteration << endl;
    cout << "   Successful resiliency check: " << successful_resiliency_check << endl;
    cout << "             Successful replan: " << successful_replan << endl;
    cout << "                      Deadends: " << g_deadend_states->get_size() << endl;
    cout << "               Resilient nodes: " << resilient_nodes.size() << endl;
    cout << "           Non-resilient nodes: " << non_resilient_nodes.size() << endl;
    cout << "               Actions planned: " << g_policy->get_size() << endl;
    cout << "      Max dimension open stack: " << max_dimension_open << endl;

    cout << "\n--------------------------------------------------------------------"
         << endl;
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
    cout << "\n--------------------------------------------------------------------"
         << endl;
}

/// @brief Print memory statistics
void print_memory()
{
    cout << "\n                  -{ Memory Statistics }-\n"
         << endl;
    cout << "                         Start: " << g_mem_initial << " KB" << endl;
    cout << "                     Algorithm: " << g_mem_post_alg << " KB" << endl;
    cout << "                          Peak: " << get_peak_memory_in_kb() << " KB" << endl;

    cout << "\n--------------------------------------------------------------------\n"
         << endl;
}

/// @brief Print the global policy
void print_policy()
{
    cout << "\n                  -{ Global policy }-\n"
         << endl;
    g_policy->dump_state_op();
    cout << "\n--------------------------------------------------------------------\n"
         << endl;
}

/// @brief Print all resilient nodes
void print_resilient_nodes()
{
    cout << "\n                  -{ Resilient nodes }-\n"
         << endl;
    for (std::list<ResilientNode>::iterator it = resilient_nodes.begin(); it != resilient_nodes.end(); ++it)
    {
        it->dump();
        cout << endl;
    }
    cout << "\n--------------------------------------------------------------------\n"
         << endl;
}

/// @brief Print all non resilient nodes
void print_non_resilient_nodes()
{
    cout << "\n                -{ Non resilient nodes }-\n"
         << endl;
    for (std::list<ResilientNode>::iterator it = non_resilient_nodes.begin(); it != non_resilient_nodes.end(); ++it)
    {
        it->dump();
        cout << endl;
    }
    cout << "\n--------------------------------------------------------------------\n"
         << endl;
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
        cout
            << "\n\n--------------------------------------------------------------------" << endl;
        cout << "\n                  -{ Resilient plan }-\n"
             << endl;
        for (std::list<Operator>::iterator it = plan.begin(); it != plan.end(); ++it)
            cout << it->get_nondet_name() << endl;
        cout << "\n\n--------------------------------------------------------------------" << endl;
    }
}
