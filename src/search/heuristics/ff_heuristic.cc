#include "ff_heuristic.h"

#include "../globals.h"
#include "../operator.h"
#include "../option_parser.h"
#include "../plugin.h"
#include "../state.h"

#include <cassert>
#include <vector>
using namespace std;

#include <ext/hash_map>
using namespace __gnu_cxx;

// construction and destruction
FFHeuristic::FFHeuristic(const Options &opts)
    : AdditiveHeuristic(opts)
{
}

FFHeuristic::~FFHeuristic()
{
}

void FFHeuristic::reset()
{
    AdditiveHeuristic::reset();
    relaxed_plan.resize(g_operators.size(), false);
}

void FFHeuristic::initialize()
{
    if (g_verbose)
        cout << "Initializing FF heuristic..." << endl;
    AdditiveHeuristic::initialize();
    relaxed_plan.resize(g_operators.size(), false);
}

void FFHeuristic::mark_preferred_operators_and_relaxed_plan(
    const State &state, Proposition *goal)
{
    if (!goal->marked)
    {
        // Only consider each subgoal once.
        goal->marked = true;
        UnaryOperator *unary_op = goal->reached_by;
        if (unary_op)
        {
            // We have not yet chained back to a start node.
            for (int i = 0; i < unary_op->precondition.size(); i++)
                mark_preferred_operators_and_relaxed_plan(
                    state, unary_op->precondition[i]);

            int operator_no = unary_op->operator_no;

            if (operator_no != -1)
            {
                // This is not an axiom.
                relaxed_plan[operator_no] = true;

                if (unary_op->cost == unary_op->base_cost)
                {
                    // This test is implied by the next but cheaper,
                    // so we perform it to save work.
                    // If we had no 0-cost operators and axioms to worry
                    // about, it would also imply applicability.
                    const Operator *op = &g_operators[operator_no];
                    if (op->is_applicable(state))
                        set_preferred(op);
                }
            }
        }
    }
}

int FFHeuristic::compute_heuristic(const State &state)
{
    if (g_detect_deadends && (g_deadend_states->check_match(state) || g_temporary_deadends->check_match(state)))
        return DEAD_END;

    int h_add = compute_add_and_ff(state);
    if (h_add == DEAD_END)
    {
        if (g_record_online_deadends && !g_limit_states)
        {
            PartialState *de = new PartialState(state);
            if (g_generalize_deadends)
                generalize_deadend(*de);
            g_temporary_deadends->add_item(new NondetDeadend(de));
            // HAZ: For now, we forget about trying to record the context
            //      for the deadend. This means that we may not have any
            //      forbidden state-action pairs created.
            g_found_deadends.push_back(new DeadendTuple(de, NULL, NULL));
        }
        return h_add;
    }

    set<int> reaching;

    // Collecting the relaxed plan also sets the preferred operators.
    for (int i = 0; i < goal_propositions.size(); i++)
    {
        Proposition *goal = goal_propositions[i];
        if(goal->reached_by != NULL)
            reaching.insert(goal->reached_by->operator_no);
        mark_preferred_operators_and_relaxed_plan(state, goal_propositions[i]);
    }

    int h_ff = 0;

    for (int op_no = 0; op_no < relaxed_plan.size(); op_no++)
    {
        if (relaxed_plan[op_no])
        {
            relaxed_plan[op_no] = false; // Clean up for next computation.
            h_ff += get_adjusted_cost(g_operators[op_no]);
        }
    }

    return h_ff;
}

static Heuristic *_parse(OptionParser &parser)
{
    parser.document_synopsis("FF heuristic", "See also Synergy.");
    parser.document_language_support("action costs", "supported");
    parser.document_language_support("conditional_effects", "supported");
    parser.document_language_support(
        "axioms",
        "supported (in the sense that the planner won't complain -- "
        "handling of axioms might be very stupid "
        "and even render the heuristic unsafe)");
    parser.document_property("admissible", "no");
    parser.document_property("consistent", "no");
    parser.document_property("safe", "yes for tasks without axioms");
    parser.document_property("preferred operators", "yes");

    Heuristic::add_options_to_parser(parser);
    Options opts = parser.parse();
    if (parser.dry_run())
        return 0;
    else
        return new FFHeuristic(opts);
}

static Plugin<Heuristic> _plugin("ff", _parse);
