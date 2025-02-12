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
#include "resilient_node_formula.h"
#include "resilient_policy.h"
#include "print_utils.h"
#include "landmarks/exploration.h"
#include "landmarks/landmark_factory_zhu_givan.h"
#include "landmarks/h_m_landmarks.h"


#include <math.h>
#include <list>
#include <string>
#include <vector>
#include <stack>
#include <tr1/functional>
#include <tr1/unordered_map>



using namespace std;
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
    bool safe;
    int cost;
    int unsatisfied_preconditions;
    int h_max_supporter_cost; // h_max_cost of h_max_supporter
    RelaxedProposition *h_max_supporter;
    RelaxedOperator(const std::vector<RelaxedProposition *> &pre,
                    const std::vector<RelaxedProposition *> &eff,
                    const Operator *the_op, int base)
        : op(the_op), precondition(pre), effects(eff), base_cost(base), safe(false)
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
    bool safe;
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
bool prune_check(ResilientNode node);
PartialState regression(PartialState from, const Operator* op);
bool resiliency_check(ResilientNode node);
bool resiliency_check_formula(ResilientNode node);
bool replan(ResilientNode current_node, SearchEngine *engine);
std::list<Operator> extract_solution();
void update_non_resilient_nodes(ResilientNode node);
void add_non_resilient_deadends(ResilientNode node);
Operator generate_macro_action(PartialState partial_state);
std::vector<PartialState> partial_state_to_goal(std::vector<const Operator *> plan);

std::tr1::unordered_map<int, ResilientNode> resilient_nodes;
std::tr1::unordered_map<int, ResilientNodeFormula> resilient_nodes_formula;
std::tr1::unordered_map<int, ResilientNode> non_resilient_nodes;
std::stack<ResilientNode> open;
std::vector<RelaxedProposition> fragile_atoms;

int main(int argc, const char **argv)
{
    register_event_handlers();
    if (argc < 2)
    {
        exit_with(EXIT_INPUT_ERROR);
    }

    if (string(argv[1]).compare("--help") != 0){
        // TODO  qua vengono generate le operazioni
        read_everything(cin);
    }



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

    for (size_t i = 0; i < g_operators.size(); ++i){
        for (size_t y = 0; y < safe_actions.size(); ++y) {
            if (g_operators[i].get_name().find(safe_actions[y]) != string::npos)
            {
                g_operators[i].set_safe(true);
                g_operators[i].set_marked(true);
            }
        }
    }
    g_operators_backup = g_operators; // PREVIAL, SONO LE PRECONDIZIONI PREPOST, GLI EFFETTI
    /***********************
     * Resilient Alghoritm *
     ***********************/

    State static_initial_state = g_initial_state();

    // Create initial node and pushing to open stack
    ResilientNode initial_node = ResilientNode(g_initial_state(), g_max_faults, std::set<Operator>());
    g_initial_node_id = initial_node.get_id();
    open.push(initial_node);
    g_timer_cycle.resume();

    //TODO: il pruning solo per alcuni fatti e landmark, non tutti, non ha senso!
    if(g_pruning){
        PartialState current_state = PartialState(initial_node.get_state());
        std::vector<std::vector<RelaxedProposition> > propositions;
        std::vector<RelaxedOperator> relaxed_operators;
        propositions.resize(g_variable_domain.size());
        for (int var = 0; var < g_variable_domain.size(); var++){
            for (int value = 0; value < g_variable_domain[var]; value++){
                RelaxedProposition prop = RelaxedProposition();
                prop.safe = false;
                prop.name = g_fact_names[var][value];
                propositions[var].push_back(prop);
            }
        }
        for (int i = 0; i < g_operators.size(); i++){
            const vector<Prevail> &prevail = g_operators[i].get_prevail();
            const vector<PrePost> &pre_post = g_operators[i].get_pre_post();
            vector<RelaxedProposition *> precondition;
            vector<RelaxedProposition *> effects;
            for (int j = 0; j < prevail.size(); j++)
                precondition.push_back(&propositions[prevail[j].var][prevail[j].prev]);
            for (int j = 0; j < pre_post.size(); j++){
                if (pre_post[j].pre != -1)
                    precondition.push_back(&propositions[pre_post[j].var][pre_post[j].pre]);
                effects.push_back(&propositions[pre_post[j].var][pre_post[j].post]);
            }
            RelaxedProposition artificial_precondition;
            RelaxedOperator relaxed_op(precondition, effects, &g_operators[i], 0);
            if (g_operators[i].is_safe())
                relaxed_op.safe = true;
            relaxed_operators.push_back(relaxed_op);
        }
        for (int i = 0; i < relaxed_operators.size(); i++){
            RelaxedOperator *op = &relaxed_operators[i];
            for (int j = 0; j < op->precondition.size(); j++)
                op->precondition[j]->precondition_of.push_back(op);
            for (int j = 0; j < op->effects.size(); j++){
                op->effects[j]->effect_of.push_back(op);
                if (op->safe)
                    op->effects[j]->safe = true;
            }
        }
        LandmarkFactoryZhuGivan *lm_graph_factory = new LandmarkFactoryZhuGivan(landmark_generator_options);
        LandmarkGraph* landmarks_graph = lm_graph_factory->compute_lm_graph();
        std::vector<pair<int, int> > landmarks;
        landmarks = landmarks_graph->extract_landmarks();
        g_operators = g_operators_backup;
        for (int pos = 0; pos < landmarks.size(); pos++){
            std::pair<int, int> landmark = landmarks[pos];
            int var = landmark.first;
            int value = landmark.second;
            RelaxedProposition &prop = propositions[var][value];
            if (!prop.safe){
                if (current_state[var] != -1 && current_state[var] != value && g_max_faults >= prop.effect_of.size())
                {
                    g_pruning_before_all_value++;
                    open.pop();
                    break;
                }
            }
        }
    }
    while (!open.empty()){
        PartialState goal_partial_state = PartialState();
        for (int i = 0; i < g_goal.size(); i++)
        {
            (goal_partial_state)[g_goal[i].first] = g_goal[i].second;
        }
        g_iteration++;
        if (open.size() > g_max_dimension_open)
            g_max_dimension_open = open.size();
        ResilientNode current_node = open.top();
        open.pop();
        g_current_faults = current_node.get_k();
        g_current_forbidden_ops = current_node.get_deactivated_op();
        if (g_verbose){
            cout << "\n----------------------------------------" << endl;
            cout << "\nIteration:" << g_iteration << endl;
            cout << "\nCurrent node:" << endl;
            current_node.dump();
        }
        current_node.dump();
        if (resilient_nodes.find(current_node.get_id()) == resilient_nodes.end() && non_resilient_nodes.find(current_node.get_id()) == non_resilient_nodes.end())
        {
            if (resiliency_check(current_node))
            {
                g_successful_resiliency_check++;
                resilient_nodes.insert(make_pair(current_node.get_id(), current_node));
                if (g_verbose)
                    cout << "\nSuccessfull resiliency check." << endl;
            }
            else{
                if (g_verbose)
                    cout << "\nFailed resiliency check." << endl;
                // Replan function return true if successfull, the plan is stored in engine object
                if (!replan(current_node, engine)){
                    if (g_verbose)
                        cout << "\nFailed replan." << endl;
                    update_non_resilient_nodes(current_node); // R downarrow
                }
                else{
                    g_successful_replan++;
                    if (g_verbose)
                        cout << "Successfull replanning" << endl;
                    // Save current initial state in a variable and computed plan for iteration
                    State current = g_initial_state();
                    std::vector<const Operator *> plan = engine->get_plan();
                    bool prune = false;
                    if (current_node.get_k() >= 1)
                    {
                        for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                        {
                            // Create node <tau_i-1, k, V>
                            ResilientNode res_node = ResilientNode(current, g_current_faults, g_current_forbidden_ops);
                            // Create node <tau_i-1, k - 1, V U {pi_i}>
                            if(g_pruning){
                                if(prune_check(res_node)){
                                    update_non_resilient_nodes(res_node);
                                    std::set<Operator> post_actions = g_current_forbidden_ops;
                                    post_actions.insert(*(*it)); // *it = pi_i
                                    ResilientNode res_node_f = ResilientNode(current, g_current_faults - 1, post_actions);
                                    update_non_resilient_nodes(res_node_f);
                                    prune = true;
                                    break;
                                }
                                else{
                                    open.push(res_node);
                                    if (!(*it)->is_safe())
                                    {
                                        std::set<Operator> post_actions = g_current_forbidden_ops;
                                        post_actions.insert(*(*it)); // *it = pi_i
                                        ResilientNode res_node_f = ResilientNode(current, g_current_faults - 1, post_actions);
                                        open.push(res_node_f);
                                    }
                                }
                            }else{
                                open.push(res_node);
                                if (!(*it)->is_safe())
                                {
                                    std::set<Operator> post_actions = g_current_forbidden_ops;
                                    post_actions.insert(*(*it)); // *it = pi_i
                                    ResilientNode res_node_f = ResilientNode(current, g_current_faults - 1, post_actions);
                                    open.push(res_node_f);
                                }
                            }
                            current = g_state_registry->get_successor_state(current, *(*it));
                        }
                        if(!prune){
                            ResilientNode tau = ResilientNode(current, g_current_faults, g_current_forbidden_ops);
                            resilient_nodes.insert(make_pair(tau.get_id(), tau));
                        }
                        // //TODO aggiungo stato goal a Rfrecciasu 
                        if (!prune)
                        {
                            ResilientNodeFormula tau = ResilientNodeFormula(goal_partial_state, g_current_faults, g_current_forbidden_ops);
                            resilient_nodes_formula.insert(make_pair(tau.get_id(), tau));
                        }
                    }
                    else {
                        //TODO generazione regressione
                        PartialState formula = goal_partial_state;
                        ResilientNodeFormula res_formula = ResilientNodeFormula(formula, 0, current_node.get_deactivated_op());
                        resilient_nodes_formula.insert(std::make_pair(res_formula.get_id(), res_formula));
                        for (size_t i = 0; i < plan.size(); ++i)
                        {
                            const Operator* op = const_cast<Operator*>(plan[plan.size()-i-1]);
                            formula = regression(formula, op);
                            res_formula = ResilientNodeFormula(formula, 0, current_node.get_deactivated_op());
                            resilient_nodes_formula.insert(std::make_pair(res_formula.get_id(), res_formula));
                        }
                        for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                        {
                            ResilientNode res_node = ResilientNode(current, 0, current_node.get_deactivated_op());
                            resilient_nodes.insert(std::make_pair(res_node.get_id(), res_node));
                            relation_node_next_action[res_node.get_id()] = *(*it);
                            current = g_state_registry->get_successor_state(current, *(*it));
                        }
                        ResilientNode tau = ResilientNode(current, 0, current_node.get_deactivated_op());
                        resilient_nodes.insert(make_pair(tau.get_id(), tau));
                    }
                    regression_steps.clear();
                    regression_steps = perform_regression(plan, g_matched_policy, 0, true);
                    // Update global policy with the new plan
                    g_policy->update_policy(regression_steps);
                    // Save policy relative to last computed plan mapping it with current <k,V>
                    Policy *resilient_policy = new Policy();
                    resilient_policy->update_policy(regression_steps);
                    g_resilient_policies.insert(std::make_pair(std::make_pair(g_current_faults, g_current_forbidden_ops), resilient_policy));
                    if (g_verbose){
                        cout << "Plan:" << endl;
                        resilient_policy->dump_simple();
                    }
                }
            }
        }
        if (g_max_iterations > 0 && g_iteration >= g_max_iterations)
            break;
        if (g_verbose){
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
            res_policy.extract_policy(static_initial_state, *(g_policy->get_items().front()->state), g_max_faults, resilient_nodes);
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

PartialState regression(PartialState from, const Operator* op){
    PartialState regressed = PartialState(from, *op, false, new PartialState());
    return regressed;
}

Operator generate_macro_action(PartialState partial_state){
    vector<Prevail> formula_previal;
    PartialState goal_partial_state = PartialState();
    for (int i = 0; i < g_goal.size(); i++)
    {
        (goal_partial_state)[g_goal[i].first] = g_goal[i].second;
    }
    for (size_t i = 0; i < g_variable_domain.size(); ++i)
    {
        if(partial_state[i]!=-1){
            Prevail formula_pre = Prevail(i, partial_state[i]);
            formula_previal.push_back(formula_pre);
        }
    }

    std::vector<PrePost> pre_post;
    vector<Prevail> dummy;
    for (size_t i = 0; i < g_variable_domain.size(); ++i)
    {
        if(goal_partial_state[i] != -1){
            PrePost state_post = PrePost(i, -1, goal_partial_state[i], dummy);
            pre_post.push_back(state_post);
        }
    }

    // Operator* macro = new Operator(formula_previal, pre_post, "macro");
    Operator macro = Operator(formula_previal, pre_post, "macro");

    return macro;
}

bool prune_check(ResilientNode node){
    PartialState current_state = PartialState(node.get_state());
    g_state_registry->reset_initial_state();
    for (int i = 0; i < g_variable_name.size(); i++)
        g_initial_state_data[i] = current_state[i];
    for (int i = 0; i < g_operators.size(); i++)
    {
        if (g_current_forbidden_ops.find(g_operators[i]) != g_current_forbidden_ops.end())
            g_operators.erase(g_operators.begin() + i--);
    }
    for (int i = 0; i < g_operators.size(); i++)
        {
        if (g_current_forbidden_ops.find(g_operators[i]) != g_current_forbidden_ops.end())
            g_operators.erase(g_operators.begin() + i--);
        }
        std::vector<std::vector<RelaxedProposition> > propositions;
        std::vector<RelaxedOperator> relaxed_operators;
        propositions.resize(g_variable_domain.size());
        for (int var = 0; var < g_variable_domain.size(); var++)
        {
            for (int value = 0; value < g_variable_domain[var]; value++)
            {
                RelaxedProposition prop = RelaxedProposition();
                prop.name = g_fact_names[var][value];
                propositions[var].push_back(prop);
            }
        }
        for (int i = 0; i < g_operators.size(); i++)
        {
            const vector<Prevail> &prevail = g_operators[i].get_prevail();
            const vector<PrePost> &pre_post = g_operators[i].get_pre_post();
            vector<RelaxedProposition *> precondition;
            vector<RelaxedProposition *> effects;
            for (int j = 0; j < prevail.size(); j++)
                precondition.push_back(&propositions[prevail[j].var][prevail[j].prev]);
            for (int j = 0; j < pre_post.size(); j++)
            {
                if (pre_post[j].pre != -1)
                    precondition.push_back(&propositions[pre_post[j].var][pre_post[j].pre]);
                effects.push_back(&propositions[pre_post[j].var][pre_post[j].post]);
            }
            RelaxedProposition artificial_precondition;
            RelaxedOperator relaxed_op(precondition, effects, &g_operators[i], 0);
            relaxed_operators.push_back(relaxed_op);
        }
        for (int i = 0; i < relaxed_operators.size(); i++){
            RelaxedOperator *op = &relaxed_operators[i];
            for (int j = 0; j < op->precondition.size(); j++)
                op->precondition[j]->precondition_of.push_back(op);
            for (int j = 0; j < op->effects.size(); j++){
                op->effects[j]->effect_of.push_back(op);
                if (op->safe)
                    op->effects[j]->safe = true;
            }
        }
        LandmarkFactoryZhuGivan *lm_graph_factory = new LandmarkFactoryZhuGivan(landmark_generator_options);
        LandmarkGraph* landmarks_graph = lm_graph_factory->compute_lm_graph();
        std::vector<pair<int, int> > landmarks;
        landmarks = landmarks_graph->extract_landmarks();
        g_operators = g_operators_backup;
        for (int pos = 0; pos < landmarks.size(); pos++)
        {
            std::pair<int, int> landmark = landmarks[pos];
            int var = landmark.first;
            int value = landmark.second;
            RelaxedProposition &prop = propositions[var][value];
            if (!prop.safe){
                if (current_state[var] != -1 && current_state[var] != value && g_current_faults >= prop.effect_of.size())
                {
                    g_pruning_before_planning_value++;
                    return false;
                }
            }
        }
    return false;
}

bool resiliency_check_formula(ResilientNode node)
{
    if (resilient_nodes_formula.empty())
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
    PartialState current_r = PartialState(state); 
    StateRegistry *registry = const_cast<StateRegistry *>(&state.get_registry());

    // Resiliency check cycle
    for (std::set<Operator>::iterator it_o = next_actions.begin(); it_o != next_actions.end(); ++it_o)
    {
        State successor = registry->get_successor_state(node.get_state(), *it_o);
        PartialState successor_p = PartialState(successor);
        // ResilientNode successor_r = ResilientNode(successor, node.get_k(), node.get_deactivated_op()); // <s[a], k, V>

        std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
        forbidden_plus_current.insert(*it_o);

        // TODO DOPPIO FOR INNESTATO PRIMA SUL SUCCESSORE
        for (std::tr1::unordered_map<int, ResilientNodeFormula>::iterator it = resilient_nodes_formula.begin(); it != resilient_nodes_formula.end(); ++it) {
            ResilientNodeFormula successor_node_formula = it->second;
            if (successor_node_formula.get_formula().is_model(successor_p) && successor_node_formula.get_k() == node.get_k() && successor_node_formula.get_deactivated_op() == node.get_deactivated_op())
            {
                for (std::tr1::unordered_map<int, ResilientNodeFormula>::iterator it_2 = resilient_nodes_formula.begin(); it_2 != resilient_nodes_formula.end(); ++it_2){
                    ResilientNodeFormula current_resilient_node_formula = it_2->second;
                    if (current_resilient_node_formula.get_formula().is_model(current_r) && (current_resilient_node_formula.get_k() == node.get_k() - 1) && (current_resilient_node_formula.get_deactivated_op() == forbidden_plus_current))
                    {

                        PartialState formula = current_resilient_node_formula.get_formula();
                        ResilientNodeFormula to_add = ResilientNodeFormula(formula, node.get_k(), node.get_deactivated_op());
                        resilient_nodes_formula.insert(make_pair(to_add.get_id(), to_add));
                        return true;
                    }
                }
            }
        }
    }
    return false;
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

        if(it_o->is_safe())
            if (resilient_nodes.find(successor_r.get_id()) != resilient_nodes.end())
            {
                relation_node_next_action[node.get_id()] = *it_o;
                return true;
            }
        if (resilient_nodes.find(successor_r.get_id()) != resilient_nodes.end() && resilient_nodes.find(current_r.get_id()) != resilient_nodes.end())
            if (resilient_nodes.find(successor_r.get_id()) != resilient_nodes.end())
            {
                relation_node_next_action[node.get_id()] = *it_o;
                return true;
            }
    }
    return false;
}


// the idea behind this function is generate a partial state to goal
std::vector<PartialState> partial_state_to_goal(std::vector<const Operator *> plan){
    std::vector<PartialState> partial_states;
    PartialState first_regression_state;
    PartialState current_regression_state;
    for (int i = 0; i < g_goal.size(); i++)
    {
        first_regression_state[g_goal[i].first] = g_goal[i].second;
    }
    for (size_t i = 0; i < plan[plan.size() - 1]->get_pre_post().size(); ++i)
    {
        const PrePost &pre_post = plan[plan.size()-1]->get_pre_post()[i];
        first_regression_state[pre_post.var] = pre_post.pre;
    }
    partial_states.push_back(first_regression_state);

    for (int i = plan.size(); i > 1; --i)
    {
        const Operator* oper = plan[i - 2];
        for (size_t i = 0; i < oper->get_pre_post().size(); ++i)
        {
            const PrePost &pre_post = oper->get_pre_post()[i];
            first_regression_state[pre_post.var] = pre_post.pre;
        }
        partial_states.push_back(first_regression_state);
        first_regression_state.dump_pddl();
    }
    return partial_states;
}

bool replan(ResilientNode current_node, SearchEngine *engine){
    PartialState current_state = PartialState(current_node.get_state());
    g_macro_actions.clear();
    g_state_registry->reset_initial_state();
    for (int i = 0; i < g_variable_name.size(); i++)
        g_initial_state_data[i] = current_state[i];
    for (int i = 0; i < g_operators.size(); i++){
        if (g_current_forbidden_ops.find(g_operators[i]) != g_current_forbidden_ops.end())
            g_operators.erase(g_operators.begin() + i--);
    }
    
    for (std::tr1::unordered_map<int, ResilientNodeFormula>::iterator it = resilient_nodes_formula.begin(); it != resilient_nodes_formula.end(); ++it)
    {
        ResilientNodeFormula node_formula = it->second;
        PartialState goal_partial_state = PartialState();
        for (int i = 0; i < g_goal.size(); i++)
        {
            (goal_partial_state)[g_goal[i].first] = g_goal[i].second;
        }
        if(!node_formula.get_formula().is_model(goal_partial_state)){
            Operator macro = generate_macro_action(node_formula.get_formula());
            g_macro_actions.push_back(macro);
        }
    }
    
    g_timer_engine_init.resume();
    engine->reset();
    g_timer_engine_init.stop();

    g_timer_search.resume();
    engine->search();

    g_timer_search.stop();
    g_macro_actions.clear();
    if (g_dump_memory_replan_progression)
        cout << "Memory at replan #" << g_replan_counter + 1 << ": " << mem_usage() << "KB" << endl;
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
    ResilientNode this_node = ResilientNode(node.get_state(), node.get_k(), node.get_deactivated_op());

    non_resilient_nodes.insert(make_pair(node.get_id(), node));
    add_non_resilient_deadends(this_node);

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
            if (to_add.get_id() != g_initial_node_id){
                non_resilient_nodes.insert(make_pair(to_add.get_id(), to_add));
                add_non_resilient_deadends(to_add);
            }

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
        de_items.push_back(new NondetDeadend(new PartialState(*de_state, *(ro->op), false, dummy_state), ro->op->nondet_index));
    }

    delete dummy_state;
    Policy *current_deadend_policy = new Policy();
    if (g_non_resilient_deadends.find(std::make_pair(node.get_k(), node.get_deactivated_op())) != g_non_resilient_deadends.end()){
        current_deadend_policy = g_non_resilient_deadends[std::make_pair(node.get_k(), node.get_deactivated_op())];
    }
    current_deadend_policy->update_policy(de_items);
    g_non_resilient_deadends.insert(std::make_pair(std::make_pair(node.get_k(), node.get_deactivated_op()), current_deadend_policy));

    std::set<Operator> v = node.get_deactivated_op();
    for (std::set<Operator>::iterator it = v.begin(); it != v.end(); ++it)
    {
        std::set<Operator> forbidden_minus_a = node.get_deactivated_op();
        forbidden_minus_a.erase(*it);
        Policy *s_a = new Policy();
        list<PolicyItem *> s_a_item;

        s_a_item.push_back(new NondetDeadend(new PartialState(state), it->nondet_index));

        if (g_non_resilient_deadends.find(std::make_pair(node.get_k() + 1, forbidden_minus_a)) != g_non_resilient_deadends.end())
            g_non_resilient_deadends.find(std::make_pair(node.get_k() + 1, forbidden_minus_a))->second->update_policy(s_a_item);
        else
        {
            s_a->update_policy(s_a_item);
            g_non_resilient_deadends.insert(std::make_pair(std::make_pair(node.get_k() + 1, forbidden_minus_a), s_a));
        }
    }
}
