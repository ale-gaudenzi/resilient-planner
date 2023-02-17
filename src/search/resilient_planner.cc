#include "globals.h"
#include "operator.h"
#include "option_parser.h"
#include "ext/tree_util.hh"
#include "timer.h"
#include "utilities.h"
#include "search_engine.h"
#include "policy-repair/regression.h"
#include "policy-repair/simulator.h"
#include "policy-repair/policy.h"
#include "policy-repair/jit.h"
#include "policy-repair/partial_state.h"
#include "resilient_state.h"

#include <iostream>
#include <list>
#include <new>
#include <string>
using namespace std;

bool verbose = false;

// probably better take this function in a separate class in future
bool resiliency_check(ResilientState state);
void reset_goal();
bool compute_plan(ResilientState current_node, SearchEngine *engine);

// main and compute_plan work on this. is cleared before every replan
list<PolicyItem *> regression_steps;
PolicyItem *goal_step = NULL;

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
    g_timer_simulator.stop();
    g_timer_engine_init.stop();
    g_timer_search.stop();
    g_timer_policy_build.stop();
    g_timer_policy_eval.stop();
    g_timer_policy_use.stop();
    g_timer_jit.stop();

    g_timer_regression.reset();
    g_timer_simulator.reset();
    g_timer_engine_init.reset();
    g_timer_search.reset();
    g_timer_policy_build.reset();
    g_timer_policy_eval.reset();
    g_timer_policy_use.reset();
    g_timer_jit.reset();

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

    /*********************
     * Initial policy search *
     *********************/

    // We start the jit timer here since we should include the initial search / policy construction
    g_timer_jit.resume();
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

    cout << "\n\nRegressing the plan..." << endl;
    regression_steps = perform_regression(engine->get_plan(), g_matched_policy, 0, true);

    cout << "\n\nGenerating an initial policy..." << endl;
    g_policy = new Policy();
    g_policy->update_policy(regression_steps);
    g_best_policy = g_policy;
    g_best_policy_score = g_policy->get_score();

    /*
        cout << "--------------" << endl;
        cout << "INITIAL POLICY" << endl;
        g_policy->dump();
        cout << "--------------" << endl;
    */

    if (g_sample_for_depth1_deadends)
        sample_for_depth1_deadends(engine->get_plan(), new PartialState(g_initial_state()));

    /******************************************/

    // spsotati in globals gli altri set
    // set<ResilientState> deadends; // dovrebbe gestirli da solo, non credo serva

    // primo riempimento dello stack con la policy iniziale trovata
    for (list<PolicyItem *>::iterator op_iter = regression_steps.begin(); op_iter != regression_steps.end(); ++op_iter)
    {
        RegressionStep *reg_step = dynamic_cast<RegressionStep *>(*op_iter);

        std::set<Operator> post_action;
        if (!reg_step->is_goal)
        {
            post_action.insert(reg_step->get_op());
        }
        else
        {
            goal_step = reg_step;
        }

        ResilientState res_state = ResilientState(*reg_step->state, g_max_faults);                    // stato se azione successiva non fallisce
        ResilientState res_state_f = ResilientState(*reg_step->state, g_max_faults - 1, post_action); // stato se azione successiva fallisce

        post_action.clear();

        if (verbose)
        {
            res_state.dump();
            res_state_f.dump();
        }

        g_nodes.push(res_state);
        g_nodes.push(res_state_f);
    }

    // serve aggiungere stato goal a R ?
    while (!g_nodes.empty())
    {
        ResilientState current_node = g_nodes.top();
        g_nodes.pop();

        if (!resiliency_check(current_node))
        {

            // if (!compute_plan(current_node, engine)) //TODO make computePlan works with new current_node as initial state
            if (false)
            {
                // cout << "AAAAAAAAAAA" << endl;
                //  add node to deadend,
                //  add regr to F
                //  TODO: ask Diego about deadend logic and F if we can control only forbidden action V
            }
            /*
            else if (current_node.get_k() >= 1)
            {
                std::list<PolicyItem *>::iterator it;
                for (it = regression_steps.begin(); it != regression_steps.end(); ++it)
                { // TODO: ask Diego if the order matter
                    PolicyItem *item = *it;
                    RegressionStep *reg_step = (RegressionStep *)&item;

                    ResilientState state_to_add = *item->state;

                    ResilientState state_to_push_A = ResilientState(state_to_add, current_node.get_k(), current_node.get_deactivated_op());
                    g_nodes.push(state_to_push_A);

                    std::set<Operator> set_B = current_node.get_deactivated_op();
                    set_B.insert(*reg_step->op);
                    ResilientState state_to_push_B = ResilientState(state_to_add, current_node.get_k() - 1, set_B);
                    g_nodes.push(state_to_push_B);
                }
            }

            else
            {
                std::list<PolicyItem *>::iterator it;
                for (it = regression_steps.begin(); it != regression_steps.end(); ++it)
                { // TODO: ask Diego if the order matter
                    PolicyItem *item = *it;
                    ResilientState state_to_add = ResilientState(*item->state, 0, current_node.get_deactivated_op());
                    g_resilient_states.insert(state_to_add);
                }
            }*/
        }
    }

    /******************************************/

    g_timer_jit.stop();
    if (g_policy && g_best_policy && (g_best_policy != g_policy))
    {
        if (g_best_policy->get_score() > g_policy->get_score())
            g_policy = g_best_policy;
    }
    if (g_policy->is_strong_cyclic())
        exit_with(EXIT_STRONG_CYCLIC);
    else
        exit_with(EXIT_NOT_STRONG_CYCLIC);
}

/*********************
 *       RCheck       *
 *********************/
bool resiliency_check(ResilientState state)
{
    PartialState toCheck = PartialState(state);

    if (g_resilient_states.find(state) != g_resilient_states.end())
        return true;

    std::set<Operator> next_actions;

    // consider state successive action in the policy excluding the forbidden ones
    for (std::list<PolicyItem *>::iterator it_r = regression_steps.begin(); it_r != regression_steps.end(); ++it_r)
    {
        RegressionStep *reg_step = dynamic_cast<RegressionStep *>(*it_r);
        if (reg_step != goal_step)
        {
            if (state.get_deactivated_op().find(reg_step->get_op()) == state.get_deactivated_op().end() && reg_step->state == &toCheck)
            {
                next_actions.insert(reg_step->get_op());
            }
        }
    }

    // resiliency check
    for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
    {
        PartialState *successor = new PartialState(toCheck, *it_o);
        ResilientState *successor_r = new ResilientState(*successor, state.get_k(), state.get_deactivated_op());

        std::set<Operator> forbidden_plus_current = state.get_deactivated_op();
        forbidden_plus_current.insert(*it_o);
        ResilientState *successor_r2 = new ResilientState(toCheck, state.get_k() - 1, forbidden_plus_current);

        if ((g_resilient_states.find(*successor_r) == g_resilient_states.end() || successor == goal_step->state) && g_resilient_states.find(*successor_r2) == g_resilient_states.end())
        {
            g_resilient_states.insert(state);
            return true;
        }
    }
    return false;
}

/*********************
 *    computePlan     *
 *********************/

bool compute_plan(ResilientState current_node, SearchEngine *engine)
{
    PartialState *old_initial_state = new PartialState(g_initial_state());

    PartialState current_state = (PartialState)current_node;

    g_state_registry->reset_initial_state();
    for (int i = 0; i < g_variable_name.size(); i++)
        g_initial_state_data[i] = current_state[i];

    cout << "AAAAAAAAAAAAAAAAAA" << endl;
    cout << g_initial_state_data << endl;
    cout << g_initial_state().get_id() << endl;

    bool engine_ready = true;

    if (g_deadend_states->check_match(current_state, false))
        return false;
    if (is_deadend(current_state))
        return false;

    g_timer_engine_init.resume();
    try
    {
        engine->reset();
    }
    catch (SolvableError &se)
    {
        if (!g_silent_planning)
            cout << se;
        engine = 0; // Memory leak seems necessary --> engine can't be deleted.
        engine_ready = false;
    }
    g_timer_engine_init.stop();

    if (engine_ready)
    {
        if (verbose)
            cout << "Searching for a solution." << endl;

        g_timer_search.resume();
        engine->search();
        g_timer_search.stop();
        g_limit_states = false;
        return false;

        if (engine->found_solution())
        {
            if (verbose)
            {
                engine->save_plan_if_necessary();
                engine->statistics();
                engine->heuristic_statistics();
            }

            if (verbose)
                cout << "Building the regression list." << endl;

            regression_steps = perform_regression(engine->get_plan(), g_matched_policy, g_matched_distance, g_policy->empty());

            if (verbose)
                cout << "Updating the policy." << endl;

            g_policy->update_policy(regression_steps);

            reset_goal();
            g_state_registry->reset_initial_state();
            for (int i = 0; i < g_variable_name.size(); i++)
                g_initial_state_data[i] = (*old_initial_state)[i];
            return true;
        }
        else
        {
            reset_goal();
            g_state_registry->reset_initial_state();
            for (int i = 0; i < g_variable_name.size(); i++)
                g_initial_state_data[i] = (*old_initial_state)[i];
            return false;
        }
    }
    else
    {
        if (verbose)
            cout << "Replanning failed!" << endl;
        reset_goal();
        g_state_registry->reset_initial_state();
        for (int i = 0; i < g_variable_name.size(); i++)
            g_initial_state_data[i] = (*old_initial_state)[i];
        return false;
    }
}

void reset_goal()
{
    g_goal.clear();
    for (int i = 0; i < g_goal_orig.size(); i++)
        g_goal.push_back(make_pair(g_goal_orig[i].first, g_goal_orig[i].second));
}
