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
using namespace std;


int main(int argc, const char **argv) {
    register_event_handlers();

    if (argc < 2) {
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

    //the input will be parsed twice:
    //once in dry-run mode, to check for simple input errors,
    //then in normal mode
    g_timer_engine_init.resume();
    try {
        OptionParser::parse_cmd_line(argc, argv, true);
        engine = OptionParser::parse_cmd_line(argc, argv, false);
    } catch (ParseError &pe) {
        cerr << pe << endl;
        exit_with(EXIT_INPUT_ERROR);
    }
    g_timer_engine_init.stop();
    
    /* HAZ: Unfortunately, this must go here (as supposed to globals.cc)
     *      since we need to know if g_detect_deadends is true or not. */
    if (g_detect_deadends) {
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
        (g_forgetpolicy && (g_jic_limit > 0))) {
            
        cout << "\n  Parameter Error: Make sure that the set of parameters is consistent.\n" << endl;
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
    
    cout << "Max time for each of the " << epochs_remaining << " epochs: " << g_jic_limit << endl << endl;
    

    /*********************
     * Initial policy search    *
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
    
    if (!engine->found_solution()) {
        cout << "No solution -- aborting repairs." << endl;
        exit(1);
    }

    cout << "\n\nRegressing the plan..." << endl;
    list<PolicyItem *> regression_steps = perform_regression(engine->get_plan(), g_matched_policy, 0, true);
    
    cout << "\n\nGenerating an initial policy..." << endl;
    g_policy = new Policy();
    g_policy->update_policy(regression_steps);
    g_best_policy = g_policy;
    g_best_policy_score = g_policy->get_score();

    cout << "--------------" << endl;
    cout << "INITIAL POLICY" << endl;
    g_policy->dump();
    cout << "--------------" << endl;

    if (g_sample_for_depth1_deadends) sample_for_depth1_deadends(engine->get_plan(), new PartialState(g_initial_state()));


    /******************************************/
    


    // in globals?
    set<ResilientState> resilient_states; // in globals?
    set<ResilientState> deadends; // dovrebbe gestirli da solo, non credo serva
    stack<ResilientState> nodes; 

    for (list<PolicyItem *>::iterator op_iter = regression_steps.begin(); op_iter != regression_steps.end(); ++op_iter) {
        PolicyItem *item = *op_iter;
        ResilientState res_state = ResilientState(*item->state);
        res_state.dump();
        nodes.push(res_state);
    }

    while(!nodes.empty()) {
        ResilientState res_state = nodes.top(); 
        nodes.pop();

        /*
        if(resiliency_check()) {
            cout << "." << endl;
        }*/
    }





    /******************************************/

    g_timer_jit.stop();
    // Use the best policy found so far
    if (g_policy && g_best_policy && (g_best_policy != g_policy)) {
        if (g_best_policy->get_score() > g_policy->get_score())
            g_policy = g_best_policy;
    }
    if (g_policy->is_strong_cyclic())
        exit_with(EXIT_STRONG_CYCLIC);
    else
        exit_with(EXIT_NOT_STRONG_CYCLIC);
}

// togliere dal main?
/*
bool resiliency_check() {
    return false;
}
*/