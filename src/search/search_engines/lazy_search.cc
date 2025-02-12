#include "lazy_search.h"

#include "../evaluators/g_evaluator.h"
#include "../heuristics/heuristic.h"
#include "../successor_generator.h"
#include "../deadend.h"
#include "../evaluators/sum_evaluator.h"
#include "../evaluators/weighted_evaluator.h"
#include "../plugin.h"
#include "../landmarks/landmark_factory_zhu_givan.h"
#include "../landmarks/h_m_landmarks.h"

#include <algorithm>
#include <limits>

static const int DEFAULT_LAZY_BOOST = 1000;


class RelaxedProposition;
class RelaxedOperator;
/// @brief Try to replan from the state contained in current_node to the goal.
/// The search engine is resetted and not recreated to avoid the overhead of the initialization.
/// The plan is saved inside the engine object and can be retrieved from it.
/// @param current_node The node containing the state to replan from.
/// @param engine The search engine originally created.
/// @return True if the replan succeds, false otherwise.
enum PropositionStatus
{
    UNREACHED = 0,
    REACHED = 1,
    GOAL_ZONE = 2,
    BEFORE_GOAL_ZONE = 3
};
struct RelaxedOperator
{
    const Operator *op;
    std::vector<RelaxedProposition *> precondition;
    std::vector<RelaxedProposition *> effects;
    int base_cost; // 0 for axioms, 1 for regular operators

    int cost;
    int unsatisfied_preconditions;
    int h_max_supporter_cost; // h_max_cost of h_max_supporter
    RelaxedProposition *h_max_supporter;
    RelaxedOperator(const std::vector<RelaxedProposition *> &pre,
                    const std::vector<RelaxedProposition *> &eff,
                    const Operator *the_op, int base)
        : op(the_op), precondition(pre), effects(eff), base_cost(base)
    {
    }

    inline void update_h_max_supporter();
};
struct RelaxedProposition
{
    std::vector<RelaxedOperator *> precondition_of;
    std::vector<RelaxedOperator *> effect_of;
    string name; // for debugging
    PropositionStatus status;
    int h_max_cost;
    /* TODO: Also add the rpg depth? The Python implementation used
       this for tie breaking, and it led to better landmark extraction
       than just using the cost. However, the Python implementation
       used a heap for the priority queue whereas we use a bucket
       implementation [NOTE: no longer true], which automatically gets
       a lot of tie-breaking by depth anyway (although not complete
       tie-breaking on depth -- if we add a proposition from
       cost/depth (4, 9) with (+1,+1), we'll process it before one
       which is added from cost/depth (5,5) with (+0,+1). The
       disadvantage of using depth is that we would need a more
       complicated open queue implementation -- however, in the unit
       action cost case, we might exploit that we never need to keep
       more than the current and next cost layer in memory, and simply
       use two bucket vectors (for two costs, and arbitrarily many
       depths). See if the init h values degrade compared to Python
       without explicit depth tie-breaking, then decide.
    */

    RelaxedProposition()
    {
    }
};
std::vector<std::vector<RelaxedProposition> > propositions;
std::vector<RelaxedOperator> relaxed_operators;
std::vector<pair<int, int> > landmarks;
typedef __gnu_cxx::hash_set<std::pair<int, int>, hash_int_pair> lm_set;
class plan_graph_node {
public:
        lm_set labels;
        inline bool reached() const {
            // NOTE: nodes are always labeled with itself,
            // if they have been reached
            return !labels.empty();
        }
};


LazySearch::LazySearch(const Options &opts)
    : SearchEngine(opts),
      open_list(opts.get<OpenList<OpenListEntryLazy> *>("open")),
      reopen_closed_nodes(opts.get<bool>("reopen_closed")),
      succ_mode(pref_first),
      current_state(g_initial_state()),
      current_predecessor_id(StateID::no_state),
      current_operator(NULL),
      current_g(0),
      current_real_g(0),
      was_initialized(false)
{
}

LazySearch::~LazySearch()
{
}

void LazySearch::reset()
{
    /**
     * For resilient planner
     * Removing operators in V from g_operators before reinitializing the heuristic
     */

    SearchEngine::reset();
    initialize();
    current_state = g_initial_state();

    current_predecessor_id = StateID::no_state;
    current_operator = NULL;
    current_g = 0;
    current_real_g = 0;
    state_count = 0;

    open_list->clear();

    for (int i = 0; i < heuristics.size(); i++)
        heuristics[i]->reset();
    
    /**
     * For resilient planner
     * Restore the original operators after heuristic reinitialization
     */
    g_operators = g_operators_backup;
}

void LazySearch::set_pref_operator_heuristics(
    vector<Heuristic *> &heur)
{
    preferred_operator_heuristics = heur;
}

void LazySearch::initialize()
{
    // TODO children classes should output which kind of search
    if (g_verbose)
        cout << "Conducting lazy best first search, (real) bound = " << bound << endl;

    // Only set up the heuristics on the first go
    if (was_initialized)
        return;
    else
        was_initialized = true;

    assert(open_list != NULL);
    set<Heuristic *> hset;
    open_list->get_involved_heuristics(hset);

    for (set<Heuristic *>::iterator it = hset.begin(); it != hset.end(); it++)
    {
        estimate_heuristics.push_back(*it);
        search_progress.add_heuristic(*it);
    }

    // add heuristics that are used for preferred operators (in case they are
    // not also used in the open list)
    hset.insert(preferred_operator_heuristics.begin(),
                preferred_operator_heuristics.end());

    for (set<Heuristic *>::iterator it = hset.begin(); it != hset.end(); it++)
    {
        heuristics.push_back(*it);
    }
    assert(!heuristics.empty());
}

void LazySearch::get_successor_operators(vector<const Operator *> &ops)
{
    bool not_fast_search = true;
    vector<const Operator *> all_operators;
    vector<const Operator *> preferred_operators;


    for (int i =0; i<g_macro_actions.size();i++)
    {
        if(g_macro_actions[i]->is_applicable(current_state)){
            if (g_current_forbidden_ops.find(*g_macro_actions[i]) == g_current_forbidden_ops.end())
            {   
                ops.push_back(g_macro_actions[i]);
                not_fast_search = false;
                break;
            }
        }
    }
    if(not_fast_search){
        g_successor_generator->generate_applicable_ops(
            current_state, all_operators);

        for (int i = 0; i < preferred_operator_heuristics.size(); i++)
        {
            Heuristic *heur = preferred_operator_heuristics[i];
            if (!heur->is_dead_end())
                heur->get_preferred_operators(preferred_operators);
        }

        /**
         * For resilient planner
         * Remove from preferred operators the ones in V
         * we have already ignored them in the DeadendAwareSuccessorGenerator but
         * it seems that the heuristic still has them in its preferred_operators
         */
        for (int i = 0; i < preferred_operators.size(); i++)
        {
            if (g_current_forbidden_ops.find(*preferred_operators[i]) != g_current_forbidden_ops.end())
            {
                preferred_operators.erase(preferred_operators.begin() + i);
                i--;
            }
        }

        if (succ_mode == pref_first)
        {
            for (int i = 0; i < preferred_operators.size(); i++)
            {
                if (!preferred_operators[i]->is_marked())
                {
                    ops.push_back(preferred_operators[i]);
                    preferred_operators[i]->mark();
                }
            }

            for (int i = 0; i < all_operators.size(); i++)
            {
                if (!all_operators[i]->is_marked())
                    ops.push_back(all_operators[i]);
            }
        }
        else
        {
            for (int i = 0; i < preferred_operators.size(); i++)
                if (!preferred_operators[i]->is_marked())
                    preferred_operators[i]->mark();

            ops.swap(all_operators);

            if (succ_mode == shuffled)
                random_shuffle(ops.begin(), ops.end());
        }
    }
}

void LazySearch::generate_successors()
{
    vector<const Operator *> operators;
    get_successor_operators(operators);

    search_progress.inc_generated(operators.size());

    for (int i = 0; i < operators.size(); i++)
    {

        int new_g = current_g + get_adjusted_cost(*operators[i]);
        int new_real_g = current_real_g + operators[i]->get_cost();
        bool is_preferred = operators[i]->is_marked();
        if (is_preferred)
            operators[i]->unmark();

        if (new_real_g < bound)
        {
            open_list->evaluate(new_g, is_preferred);
            open_list->insert(
                make_pair(current_state.get_id(), operators[i]));
        }
    }
}

int LazySearch::fetch_next_state()
{
    if (open_list->empty())
    {
        if (!g_silent_planning)
            cout << "Completely explored state space -- no solution!" << endl;
        return FAILED;
    }
    OpenListEntryLazy next = open_list->remove_min();
    current_predecessor_id = next.first;
    current_operator = next.second;
    State current_predecessor = g_state_registry->lookup_state(current_predecessor_id);
    assert(current_operator->is_applicable(current_predecessor));
    current_state = g_state_registry->get_successor_state(current_predecessor, *current_operator);
    SearchNode pred_node = search_space.get_node(current_predecessor);
    current_g = pred_node.get_g() + get_adjusted_cost(*current_operator);
    current_real_g = pred_node.get_real_g() + current_operator->get_cost();
    return IN_PROGRESS;
}

int LazySearch::step()
{
    // Invariant:
    // - current_state is the next state for which we want to compute the heuristic.
    // - current_predecessor is a permanent pointer to the predecessor of that state.
    // - current_operator is the operator which leads to current_state from predecessor.
    // - current_g is the g value of the current state according to the cost_type
    // - current_real_g is the g value of the current state (using real costs)
    SearchNode node = search_space.get_node(current_state);

    bool reopen = reopen_closed_nodes && (current_g < node.get_g()) && !node.is_dead_end() && !node.is_new();

    if (node.is_new() || reopen){
        if (g_force_limit_states || (g_plan_locally_limited && g_limit_states)){
            if (state_count > g_limit_states_max)
                return FAILED;
            else
                state_count++;
        }

        StateID dummy_id = current_predecessor_id;
        // HACK! HACK! we do this because SearchNode has no default/copy constructor
        if (dummy_id == StateID::no_state)
            dummy_id = g_initial_state().get_id();

        State parent_state = g_state_registry->lookup_state(dummy_id);
        SearchNode parent_node = search_space.get_node(parent_state);

        for (int i = 0; i < heuristics.size(); i++){
            if (current_operator != NULL)
                heuristics[i]->reach_state(parent_state, *current_operator, current_state);
            heuristics[i]->evaluate(current_state);
            
            if (heuristics[i]->is_dead_end()){
                node.mark_as_dead_end();
                search_progress.inc_dead_ends();
                return fetch_next_state();
            }
        }
        search_progress.inc_evaluated_states();
        search_progress.inc_evaluations(heuristics.size());
        open_list->evaluate(current_g, false);


        if (!open_list->is_dead_end())
        {
            // We use the value of the first heuristic, because SearchSpace only
            // supported storing one heuristic value
            int h = heuristics[0]->get_value();

            if (reopen)
            {
                node.reopen(parent_node, current_operator);
                search_progress.inc_reopened();
            }
            else if (current_predecessor_id == StateID::no_state)
            {
                node.open_initial(h);
                search_progress.get_initial_h_values();
            }
            else
            {
                node.open(h, parent_node, current_operator);
            }

            node.close();


            if (check_goal_and_set_plan(current_state)){
                return SOLVED;
            }
                
        
            if (search_progress.check_h_progress(current_g))
                reward_progress();

            generate_successors();
            search_progress.inc_expanded();
        }
        else{
            node.mark_as_dead_end();
            search_progress.inc_dead_ends();
        }
    }

    return fetch_next_state();
}

void LazySearch::reward_progress()
{
    // Boost the "preferred operator" open lists somewhat whenever
    open_list->boost_preferred();
}

void LazySearch::statistics() const
{
    search_progress.print_statistics();
}

static SearchEngine *_parse(OptionParser &parser)
{
    parser.document_synopsis("Lazy best first search", "");
    Plugin<OpenList<OpenListEntryLazy> >::register_open_lists();
    parser.add_option<OpenList<OpenListEntryLazy> *>("open", "open list");
    parser.add_option<bool>("reopen_closed",
                            "reopen closed nodes", "false");
    parser.add_list_option<Heuristic *>(
        "preferred",
        "use preferred operators of these heuristics", "[]");
    SearchEngine::add_options_to_parser(parser);
    Options opts = parser.parse();

    LazySearch *engine = 0;
    if (!parser.dry_run())
    {
        engine = new LazySearch(opts);
        vector<Heuristic *> preferred_list =
            opts.get_list<Heuristic *>("preferred");
        engine->set_pref_operator_heuristics(preferred_list);
    }

    return engine;
}

static SearchEngine *_parse_greedy(OptionParser &parser)
{
    parser.document_synopsis("Greedy search (lazy)", "");
    parser.document_note(
        "Open lists",
        "In most cases, lazy greedy best first search uses "
        "an alternation open list with one queue for each evaluator. "
        "If preferred operator heuristics are used, it adds an "
        "extra queue for each of these evaluators that includes "
        "only the nodes that are generated with a preferred operator. "
        "If only one evaluator and no preferred operator heuristic is used, "
        "the search does not use an alternation open list "
        "but a standard open list with only one queue.");
    parser.document_note("Equivalent statements using general lazy search",
                         "\n```\n--heuristic h2=eval2\n"
                         "--search lazy_greedy([eval1, h2], preferred=h2, boost=100)\n```\n"
                         "is equivalent to\n"
                         "```\n--heuristic h1=eval1 --heuristic h2=eval2\n"
                         "--search lazy(alt([single(h1), single(h1, pref_only=true), single(h2),\n"
                         "                  single(h2, pref_only=true)], boost=100),\n"
                         "              preferred=h2)\n```\n"
                         "------------------------------------------------------------\n"
                         "```\n--search lazy_greedy([eval1, eval2], boost=100)\n```\n"
                         "is equivalent to\n"
                         "```\n--search lazy(alt([single(eval1), single(eval2)], boost=100))\n```\n"
                         "------------------------------------------------------------\n"
                         "```\n--heuristic h1=eval1\n--search lazy_greedy(h1, preferred=h1)\n```\n"
                         "is equivalent to\n"
                         "```\n--heuristic h1=eval1\n"
                         "--search lazy(alt([single(h1), single(h1, pref_only=true)], boost=1000),\n"
                         "              preferred=h1)\n```\n"
                         "------------------------------------------------------------\n"
                         "```\n--search lazy_greedy(eval1)\n```\n"
                         "is equivalent to\n"
                         "```\n--search lazy(single(eval1))\n```\n",
                         true);

    parser.add_list_option<ScalarEvaluator *>("evals", "scalar evaluators");
    parser.add_list_option<Heuristic *>(
        "preferred",
        "use preferred operators of these heuristics", "[]");
    parser.add_option<bool>("reopen_closed",
                            "reopen closed nodes", "false");
    parser.add_option<int>(
        "boost",
        "boost value for alternation queues that are restricted "
        "to preferred operator nodes",
        OptionParser::to_str(DEFAULT_LAZY_BOOST));
    SearchEngine::add_options_to_parser(parser);
    Options opts = parser.parse();

    LazySearch *engine = 0;
    if (!parser.dry_run())
    {
        vector<ScalarEvaluator *> evals =
            opts.get_list<ScalarEvaluator *>("evals");
        vector<Heuristic *> preferred_list =
            opts.get_list<Heuristic *>("preferred");
        OpenList<OpenListEntryLazy> *open;
        if ((evals.size() == 1) && preferred_list.empty())
        {
            open = new StandardScalarOpenList<OpenListEntryLazy>(evals[0],
                                                                 false);
        }
        else
        {
            vector<OpenList<OpenListEntryLazy> *> inner_lists;
            for (int i = 0; i < evals.size(); i++)
            {
                inner_lists.push_back(
                    new StandardScalarOpenList<OpenListEntryLazy>(evals[i],
                                                                  false));
                if (!preferred_list.empty())
                {
                    inner_lists.push_back(
                        new StandardScalarOpenList<OpenListEntryLazy>(evals[i],
                                                                      true));
                }
            }
            open = new AlternationOpenList<OpenListEntryLazy>(
                inner_lists, opts.get<int>("boost"));
        }
        opts.set("open", open);
        engine = new LazySearch(opts);
        engine->set_pref_operator_heuristics(preferred_list);
    }
    return engine;
}

static SearchEngine *_parse_weighted_astar(OptionParser &parser)
{
    parser.document_synopsis(
        "(Weighted) A* search (lazy)",
        "Weighted A* is a special case of lazy best first search.");
    parser.document_note(
        "Open lists",
        "In the general case, it uses an alternation open list "
        "with one queue for each evaluator h that ranks the nodes "
        "by g + w * h. If preferred operator heuristics are used, "
        "it adds for each of the evaluators another such queue that "
        "only inserts nodes that are generated by preferred operators. "
        "In the special case with only one evaluator and no preferred "
        "operator heuristics, it uses a single queue that "
        "is ranked by g + w * h. ");
    parser.document_note("Equivalent statements using general lazy search",
                         "\n```\n--heuristic h1=eval1\n"
                         "--search lazy_wastar([h1, eval2], w=2, preferred=h1,\n"
                         "                     bound=100, boost=500)\n```\n"
                         "is equivalent to\n"
                         "```\n--heuristic h1=eval1 --heuristic h2=eval2\n"
                         "--search lazy(alt([single(sum([g(), weight(h1, 2)])),\n"
                         "                   single(sum([g(), weight(h1, 2)]), pref_only=true),\n"
                         "                   single(sum([g(), weight(h2, 2)])),\n"
                         "                   single(sum([g(), weight(h2, 2)]), pref_only=true)],\n"
                         "                  boost=500),\n"
                         "              preferred=h1, reopen_closed=true, bound=100)\n```\n"
                         "------------------------------------------------------------\n"
                         "```\n--search lazy_wastar([eval1, eval2], w=2, bound=100)\n```\n"
                         "is equivalent to\n"
                         "```\n--search lazy(alt([single(sum([g(), weight(eval1, 2)])),\n"
                         "                   single(sum([g(), weight(eval2, 2)]))],\n"
                         "                  boost=1000),\n"
                         "              reopen_closed=true, bound=100)\n```\n"
                         "------------------------------------------------------------\n"
                         "```\n--search lazy_wastar([eval1, eval2], bound=100, boost=0)\n```\n"
                         "is equivalent to\n"
                         "```\n--search lazy(alt([single(sum([g(), eval1])),\n"
                         "                   single(sum([g(), eval2]))])\n"
                         "              reopen_closed=true, bound=100)\n```\n"
                         "------------------------------------------------------------\n"
                         "```\n--search lazy_wastar(eval1, w=2)\n```\n"
                         "is equivalent to\n"
                         "```\n--search lazy(single(sum([g(), weight(eval1, 2)])), reopen_closed=true)\n```\n",
                         true);

    parser.add_list_option<ScalarEvaluator *>("evals", "scalar evaluators");
    parser.add_list_option<Heuristic *>(
        "preferred",
        "use preferred operators of these heuristics", "[]");
    parser.add_option<bool>("reopen_closed", "reopen closed nodes", "true");
    parser.add_option<int>("boost",
                           "boost value for preferred operator open lists",
                           OptionParser::to_str(DEFAULT_LAZY_BOOST));
    parser.add_option<int>("w", "heuristic weight", "1");
    SearchEngine::add_options_to_parser(parser);
    Options opts = parser.parse();

    opts.verify_list_non_empty<ScalarEvaluator *>("evals");

    LazySearch *engine = 0;
    if (!parser.dry_run())
    {
        vector<ScalarEvaluator *> evals = opts.get_list<ScalarEvaluator *>("evals");
        vector<Heuristic *> preferred_list =
            opts.get_list<Heuristic *>("preferred");
        vector<OpenList<OpenListEntryLazy> *> inner_lists;
        for (int i = 0; i < evals.size(); i++)
        {
            GEvaluator *g = new GEvaluator();
            vector<ScalarEvaluator *> sum_evals;
            sum_evals.push_back(g);
            if (opts.get<int>("w") == 1)
            {
                sum_evals.push_back(evals[i]);
            }
            else
            {
                WeightedEvaluator *w = new WeightedEvaluator(
                    evals[i],
                    opts.get<int>("w"));
                sum_evals.push_back(w);
            }
            SumEvaluator *f_eval = new SumEvaluator(sum_evals);

            inner_lists.push_back(
                new StandardScalarOpenList<OpenListEntryLazy>(f_eval, false));

            if (!preferred_list.empty())
            {
                inner_lists.push_back(
                    new StandardScalarOpenList<OpenListEntryLazy>(f_eval,
                                                                  true));
            }
        }
        OpenList<OpenListEntryLazy> *open;
        if (inner_lists.size() == 1)
        {
            open = inner_lists[0];
        }
        else
        {
            open = new AlternationOpenList<OpenListEntryLazy>(
                inner_lists, opts.get<int>("boost"));
        }

        opts.set("open", open);

        engine = new LazySearch(opts);
        engine->set_pref_operator_heuristics(preferred_list);
    }
    return engine;
}

static Plugin<SearchEngine> _plugin("lazy", _parse);
static Plugin<SearchEngine> _plugin_greedy("lazy_greedy", _parse_greedy);
static Plugin<SearchEngine> _plugin_weighted_astar("lazy_wastar", _parse_weighted_astar);
