/** @file */

#include "globals.h"
#include "operator.h"
#include "option_parser.h"
#include "ext/tree_util.hh"
#include "timer.h"
#include "utilities.h"
#include "search_engines/search_engine.h"
#include "regression.h"
#include "policy.h"
#include "partial_state.h"
#include "resilient_node.h"
#include "resilient_policy.h"
#include "print_utils.h"

#include <math.h>
#include <list>
#include <string>
#include <vector>
#include <stack>
#include <tr1/functional>
#include <tr1/unordered_map>

using namespace std;

bool resiliency_check(ResilientNode node);
bool replan(ResilientNode current_node, SearchEngine *engine);
std::list<Operator> extract_solution();
void update_non_resilient_nodes(ResilientNode node);
void add_non_resilient_deadends(ResilientNode node);

std::tr1::unordered_map<int, ResilientNode> resilient_nodes;
std::tr1::unordered_map<int, ResilientNode> non_resilient_nodes;
std::stack<ResilientNode> open;

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

    g_timer_engine_init.stop();
    g_timer_engine_init.reset();
    g_timer_search.stop();
    g_timer_search.reset();
    g_timer_policy_build.stop();
    g_timer_policy_build.reset();
    g_timer_cycle.stop();
    g_timer_cycle.reset();
    g_timer_extraction.stop();
    g_timer_extraction.reset();
    g_timer_extract_policy.stop();
    g_timer_extract_policy.reset();

    g_mem_initial = mem_usage();

    // The input will be parsed twice:
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

    generate_regressable_ops();

    /* HAZ: We create the policies even if we aren't using deadends, as
     *      they may be consulted by certain parts of the code. */
    g_deadend_policy = new Policy();
    g_deadend_states = new Policy();
    g_temporary_deadends = new Policy();

    if (((g_record_online_deadends || g_generalize_deadends) && !g_detect_deadends) ||
        ((g_partial_planlocal || g_plan_locally_limited) && !g_plan_locally) ||
        (g_optimized_scd && g_jic_limit == 0) ||
        (g_forgetpolicy && g_jic_limit > 0))
    {
        cout << "\n  Parameter Error: Make sure that the set of parameters is consistent.\n\n";
        exit(0);
    }
    cout << "Total allotted time (s): " << g_jic_limit << endl;

    g_operators_backup = g_operators;

    /***********************
     * Resilient Alghoritm *
     ***********************/

    // Create initial node and pushing to open stack
    ResilientNode initial_node = ResilientNode(g_initial_state(), g_max_faults, std::set<Operator>());
    open.push(initial_node);

    g_timer_cycle.resume();

    // Start main loop
    while (!open.empty())
    {
        g_iteration++;

        if (open.size() > g_max_dimension_open)
            g_max_dimension_open = open.size();

        ResilientNode current_node = open.top();
        open.pop();

        // Store current k and V in globals to use them later during replanning
        g_current_faults = current_node.get_k();
        g_current_forbidden_ops = current_node.get_deactivated_op();

        if (g_verbose)
        {
            cout << "\n----------------------------------------" << endl;
            cout << "\nIteration:" << g_iteration << endl;
            cout << "\nCurrent node:" << endl;
            current_node.dump();
        }

        // Check if node is already in R sets
        if (resilient_nodes.find(current_node.get_id()) == resilient_nodes.end() && non_resilient_nodes.find(current_node.get_id()) == non_resilient_nodes.end())
        {
            if (resiliency_check(current_node))
            {
                g_successful_resiliency_check++;
                resilient_nodes.insert(make_pair(current_node.get_id(), current_node));

                if (g_verbose)
                    cout << "\nSuccessfull resiliency check." << endl;
            }
            else
            {
                if (g_verbose)
                    cout << "\nFailed resiliency check." << endl;

                // Replan function return true if successfull, the plan is stored in engine object
                if (!replan(current_node, engine))
                {
                    if (g_verbose)
                        cout << "\nFailed replan." << endl;
                    // If replanning fails, add current node to deadend and not resilient sets
                    add_non_resilient_deadends(current_node); // S downarrow
                    update_non_resilient_nodes(current_node); // R downarrow
                }
                else
                {
                    g_successful_replan++;
                    if (g_verbose)
                        cout << "Successfull replanning" << endl;

                    // Save current initial state in a variable and computed plan for iteration
                    State current = g_initial_state();
                    std::vector<const Operator *> plan = engine->get_plan();

                    if (current_node.get_k() >= 1)
                    {
                        // Iterate over the plan to create the new nodes to push in the stack
                        for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                        {
                            // Create node <tau_i-1, k, V>
                            ResilientNode res_node = ResilientNode(current, g_current_faults, g_current_forbidden_ops);

                            // Create node <tau_i-1, k - 1, V U {pi_i}>
                            std::set<Operator> post_actions = g_current_forbidden_ops;
                            post_actions.insert(*(*it)); // *it = pi_i
                            ResilientNode res_node_f = ResilientNode(current, g_current_faults - 1, post_actions);

                            // Push new nodes in the stack
                            open.push(res_node);
                            open.push(res_node_f);

                            if (g_verbose)
                            {
                                cout << "\nPushing in the stack with adjuntive failed:" << endl;
                                res_node_f.dump();
                            }

                            current = g_state_registry->get_successor_state(current, *(*it));
                        }
                        // Add goal to resilient nodes
                        ResilientNode tau = ResilientNode(current, g_current_faults, g_current_forbidden_ops);
                        resilient_nodes.insert(make_pair(tau.get_id(), tau));
                    }
                    else // k = 0
                    {
                        // Iterate over the plan to insert new nodes in resilient set
                        for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                        {
                            ResilientNode res_node = ResilientNode(current, 0, current_node.get_deactivated_op());
                            resilient_nodes.insert(make_pair(res_node.get_id(), res_node));

                            current = g_state_registry->get_successor_state(current, *(*it));
                        }
                    }

                    // Perform regression over the computed plan
                    regression_steps.clear();
                    regression_steps = perform_regression(plan, g_matched_policy, 0, true);

                    // Update global policy with the new plan
                    g_policy->update_policy(regression_steps);

                    // Save policy relative to last computed plan mapping it with current <k,V>
                    Policy *resilient_policy = new Policy();
                    resilient_policy->update_policy(regression_steps);
                    g_resilient_policies.insert(std::make_pair(std::make_pair(g_current_faults, g_current_forbidden_ops), resilient_policy));
                    if (g_verbose)
                    {
                        cout << "Plan:" << endl;
                        resilient_policy->dump_simple();
                    }
                }
            }
        }

        if (g_max_iterations > 0 && g_iteration >= g_max_iterations)
            break;

        if (g_verbose)
        {
            cout << "Open list dimension: " << open.size() << endl;
            cout << "Resilient nodes dimension: " << resilient_nodes.size() << endl;
        }
    }

    g_timer_cycle.stop();

    // Verify if initial node is resilient
    if (resilient_nodes.find(initial_node.get_id()) != resilient_nodes.end())
    {
        cout << "\nInitial state is resilient, problem is " << g_max_faults << "-resilient!"
             << endl;

        if (g_dump_branches)
            print_branches();

        g_timer_extraction.resume();
        print_plan(g_plan_to_file, extract_solution());
        g_timer_extraction.stop();

        g_mem_post_alg = mem_usage();

        if (g_dump_resilient_policy)
        {
            ResilientPolicy res_policy = ResilientPolicy();
            g_timer_extract_policy.resume();
            res_policy.extract_policy(g_initial_state(), *(g_policy->get_items().front()->state), g_max_faults, resilient_nodes);
            g_timer_extract_policy.stop();
            print_resilient_policy_json(res_policy.get_policy());
            g_mem_extraction = mem_usage();
        }

        if (g_dump_resilient_nodes)
            print_resilient_nodes(resilient_nodes);
    }
    else
        cout << "\nInitial state is a deadend, problem is not " << g_max_faults << "-resilient!\n";

    print_statistics(resilient_nodes.size(), non_resilient_nodes.size());

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

    // Find from the policy every action applicable in the current state, minus V
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

    // Resiliency check cycle
    for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
    {
        State successor = registry->get_successor_state(node.get_state(), *it_o);
        PartialState successor_p = PartialState(successor);
        ResilientNode successor_r = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>

        std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
        forbidden_plus_current.insert(*it_o);
        ResilientNode current_r = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current); // <s, k-1, V U {a}>

        if (resilient_nodes.find(successor_r.get_id()) != resilient_nodes.end() && resilient_nodes.find(current_r.get_id()) != resilient_nodes.end())
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

    // Reset initial state to the one contained in the node
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
        cout << "Memory at replan #" << g_replan_counter + 1 << ": " << mem_usage() << "KB" << endl;

    if (g_pruning_stop)
    {
        g_pruning_stop = false;
        return false;
    }
    else
        return engine->found_solution();
}

/// @brief Extract the final resilient plan, starting by the initial state
/// and choosing only actions that lead to resilient states until the goal
/// is reached.
/// @return The plan extracted.
std::list<Operator> extract_solution()
{
    // Consider only the resilient nodes with k = max_faults to speed up later checks
    std::tr1::unordered_map<int, ResilientNode> resilient_nodes_k;
    for (std::tr1::unordered_map<int, ResilientNode>::iterator it = resilient_nodes.begin(); it != resilient_nodes.end(); ++it)
        if (it->second.get_k() == g_max_faults)
            resilient_nodes_k.insert(*it);

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
                ResilientNode successor_node = ResilientNode(successor, g_max_faults, std::set<Operator>());
                if (resilient_nodes_k.find(successor_node.get_id()) != resilient_nodes_k.end() || goal.is_implied(successor_p))
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

/// @brief Update non resilient nodes with every <s,k',V'> such that V' is a subset of V and k' = k - |V \ V'|.
/// @param node Deadend node to insert in the list non-resilient nodes.
void update_non_resilient_nodes(ResilientNode node)
{
    non_resilient_nodes.insert(make_pair(node.get_id(), node));
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
        for (counter = 0; counter < pow_set_size; counter++)
        {
            set<Operator> current;
            for (j = 0; j < set_size; j++)
                if (counter & (1 << j))
                    current.insert(vec[j]);
            subsets.push_back(current);
        }

        // add all <s,V',k'> with V' subset of V and k' = k - |V \ V'| to non-resilient set
        for (int i = 0; i < subsets.size() - 1; i++)
        {
            set<Operator> subset = subsets[i];
            int k1 = node.get_k() + (node.get_deactivated_op().size() - subset.size());
            ResilientNode to_add = ResilientNode(node.get_state(), k1, subset);
            non_resilient_nodes.insert(make_pair(to_add.get_id(), to_add));
        }
    }
    return;
}

/// @brief Regress the state contained in node and add every state-action pair Regr(s,A)
/// to the fault model policy map indexed by the current (k,V).
/// @param node Node containing the state to regress and the current (k,V).
void add_non_resilient_deadends(ResilientNode node)
{
    State state = node.get_state();
    list<PolicyItem *> de_items;
    PartialState *dummy_state = new PartialState();

    PartialState *de_state = new PartialState(state);

    // Removed generalization in order to use other heuristics than FF and ADD.
    // Need to investigate further if it's useful for performance
    // and it will be worth to generalize to other heuristics.
    // generalize_deadend(*de_state);

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
    g_non_resilient_deadends.insert(std::make_pair(std::make_pair(g_current_faults, g_current_forbidden_ops), current_deadend_policy));

    std::set<Operator> v = node.get_deactivated_op();
    for (std::set<Operator>::iterator it = v.begin(); it != v.end(); ++it)
    {
        std::set<Operator> forbidden_minus_a = g_current_forbidden_ops;
        forbidden_minus_a.erase(*it);

        Policy *s_a = new Policy();
        list<PolicyItem *> s_a_item;

        s_a_item.push_back(new NondetDeadend(new PartialState(state), it->nondet_index));

        if (g_non_resilient_deadends.find(std::make_pair(g_current_faults + 1, forbidden_minus_a)) != g_non_resilient_deadends.end())
            g_non_resilient_deadends.find(std::make_pair(g_current_faults + 1, forbidden_minus_a))->second->update_policy(s_a_item);
        else
        {
            s_a->update_policy(s_a_item);
            g_non_resilient_deadends.insert(std::make_pair(std::make_pair(g_current_faults + 1, forbidden_minus_a), s_a));
        }
    }
}
