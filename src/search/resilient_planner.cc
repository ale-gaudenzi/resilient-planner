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
#include "resilient_node.h"
#include "search_engine.h"

#include <iostream>
#include <list>
#include <new>
#include <string>
#include <vector>
#include <stack>
#include <tr1/functional>
using namespace std;

bool verbose = true;

// probably better take this function in a separate class in future
bool resiliency_check(ResilientNode node);
bool replan(ResilientNode current_node, SearchEngine *engine);
void reset_goal();
size_t make_f_hash(int k, set<Operator> forbidden);

std::set<ResilientNode> resilient_nodes;
std::stack<ResilientNode> nodes;

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

    // spostati in globals gli altri set
    // set<ResilientState> deadends; // dovrebbe gestirli da solo, non credo serva

    State current = g_initial_state();

    std::vector<const Operator *> plan = engine->get_plan();

    // primo riempimento dello stack con la policy iniziale trovata
    for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
    {
        std::set<Operator> post_actions;
        post_actions.insert(*(*it));
        ResilientNode res_node = ResilientNode(current, g_max_faults, post_actions);
        ResilientNode res_node_f = ResilientNode(current, g_max_faults - 1, post_actions);

        if (verbose)
        {
            res_node.dump();
            res_node_f.dump();
        }

        nodes.push(res_node);
        nodes.push(res_node_f);

        current = g_state_registry->get_successor_state(current, *(*it));
    }

    // serve aggiungere stato goal a R ?
    while (!nodes.empty())
    {
        ResilientNode current_node = nodes.top();
        nodes.pop();

        g_current_faults = current_node.get_k();
        g_current_forbidden = current_node.get_deactivated_op();

        if (!resiliency_check(current_node))
        {
            if (!replan(current_node, engine))
            {
                cout << "FAILED REPLANNING" << endl;
                //size_t hash = make_f_hash(g_current_faults, g_current_forbidden);

                // TODO: regression from the state to every action
                // TODO: ask if

                // list<PolicyItem *> perform_regression(const SearchEngine::Plan &plan, RegressionStep *goal_step, 1, bool create_goal)
                //  g_fault_model.insert()
            }
            else
            {
                cout << "SUCCESSFULL REPLANNING" << endl;

                State current = g_initial_state();
                std::vector<const Operator *> plan = engine->get_plan();

                if (current_node.get_k() >= 1)
                {
                    bool first = true;
                    for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                    {
                        if (first)
                        {
                            current = g_state_registry->get_successor_state(current, *(*it));
                            first = false;
                        }
                        else
                        {
                            ResilientNode res_node = ResilientNode(current, g_current_faults, g_current_forbidden);

                            std::set<Operator> post_actions = g_current_forbidden;
                            post_actions.insert(*(*it));
                            ResilientNode res_node_f = ResilientNode(current, g_current_faults - 1, post_actions);

                            if (verbose)
                            {
                                res_node.dump();
                                res_node_f.dump();
                            }

                            nodes.push(res_node);
                            nodes.push(res_node_f);
                            current = g_state_registry->get_successor_state(current, *(*it));
                        }
                    }
                } else {
                    for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                    {
                        ResilientNode res_node = ResilientNode(current, 0, g_current_forbidden);
                        resilient_nodes.insert(res_node);
                        current = g_state_registry->get_successor_state(current, *(*it));
                    }

                    regression_steps = perform_regression(engine->get_plan(), g_matched_policy, 0, true);
                    g_policy->update_policy(regression_steps);
                }
            }
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

bool resiliency_check(ResilientNode node)
{
    PartialState state = PartialState(node.get_state());

    if (!(resilient_nodes.find(node) == resilient_nodes.end()))
        return true;

    std::set<Operator> next_actions;

    std::list<PolicyItem *> full_policy;
    g_policy->copy_relevant_items(full_policy, false);

    for (std::list<PolicyItem *>::iterator it_r = full_policy.begin(); it_r != full_policy.end(); ++it_r)
    {
        RegressionStep *reg_step = dynamic_cast<RegressionStep *>(*it_r);
        if (reg_step != goal_step)
        {
            if (node.get_deactivated_op().find(reg_step->get_op()) == node.get_deactivated_op().end() && reg_step->state == &state)
            {
                next_actions.insert(reg_step->get_op());
            }
        }
    }

    // resiliency check
    for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
    {
        State successor = g_state_registry->get_successor_state(node.get_state(), *it_o);

        ResilientNode successor_r = ResilientNode(successor, node.get_k(), node.get_deactivated_op());

        std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
        forbidden_plus_current.insert(*it_o);

        ResilientNode successor_r2 = ResilientNode(node.get_state(), node.get_k() - 1, forbidden_plus_current);

        if ((resilient_nodes.find(successor_r) == resilient_nodes.end() || PartialState(successor) == *goal_step->state) && resilient_nodes.find(successor_r2) == resilient_nodes.end())
        {
            resilient_nodes.insert(node);
            return true;
        }
    }
    return false;
}

/*********************
 *    computePlan     *
 *********************/

bool replan(ResilientNode current_node, SearchEngine *engine)
{
    cout << "-----------------------" << endl;
    cout << "REPLANNING FROM " << current_node.get_state().get_id() << endl;
    bool verbose = true;
    PartialState current_state = PartialState(current_node.get_state());

    if (is_deadend(current_state))
    {
        if (verbose)
            cout << "\nDetected deadend" << endl;
        return false;
    }

    if (verbose)
        cout << "Creating initial state." << endl;

    g_state_registry->reset_initial_state();
    for (int i = 0; i < g_variable_name.size(); i++)
        g_initial_state_data[i] = current_state[i];

    if (verbose)
        cout << "Creating new engine." << endl;

    bool engine_ready = true;
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

        if (engine->found_solution())
        {
            cout << "Solution found" << endl;
            if (verbose)
            {
                engine->save_plan_if_necessary();
                engine->statistics();
                engine->heuristic_statistics();
            }

            if (verbose)
                cout << "Building the regression list." << endl;

            regression_steps.clear();
            cout << "Plan: " << endl;
            cout << engine->get_plan() << endl;

            // regression_steps = perform_regression(engine->get_plan(), g_matched_policy, g_matched_distance, g_policy->empty());
            // list<PolicyItem *> regression_steps = perform_regression(engine->get_plan(), g_matched_policy, g_matched_distance, g_policy->empty());

            if (verbose)
                cout << "Updating the policy." << endl;
            g_policy->update_policy(regression_steps);

            reset_goal();

            return true;
        }
        else
        {
            reset_goal();
            if (verbose)
                cout << "Replanning failed!" << endl;
            return false;
        }
    }
    else
    {
        if (verbose)
            cout << "Replanning failed!" << endl;
        reset_goal();
        return false;
    }
}

void reset_goal()
{
    g_goal.clear();
    for (int i = 0; i < g_goal_orig.size(); i++)
        g_goal.push_back(make_pair(g_goal_orig[i].first, g_goal_orig[i].second));
}

size_t make_f_hash(int k, set<Operator> forbidden)
{
    FaultForbiddenTuple pair = make_pair(k, forbidden);
    std::tr1::hash<int> int_hash;
    std::tr1::hash<string> string_hash;

    size_t hash1 = int_hash(k);
    for (std::set<Operator>::iterator it = forbidden.begin(); it != forbidden.end(); ++it)
    {
        string name = it->get_nondet_name();
        size_t hash2 = string_hash(name);
        hash1 = hash1 ^ hash2;
    }

    return hash1;
}
