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
    int base_cost;             // 0 for axioms, 1 for regular operators

    bool safe = false;         // 🔧 inizializzato
    int cost = 0;              // 🔧 inizializzato
    int unsatisfied_preconditions = 0;     // 🔧 inizializzato
    int h_max_supporter_cost = std::numeric_limits<int>::max(); // 🔧 inizializzato
    RelaxedProposition *h_max_supporter = nullptr;              // 🔧 inizializzato

    RelaxedOperator(const std::vector<RelaxedProposition *> &pre,
                    const std::vector<RelaxedProposition *> &eff,
                    const Operator *the_op, int base)
        : op(the_op),
          precondition(pre),
          effects(eff),
          base_cost(base)
    {
        // safe rimane a false di default; se serve lo setti dopo
        // cost/unsatisfied/... già inizializzati sopra
    }

    // esplicita i costruttori per evitare sorprese
    RelaxedOperator() = default;
    RelaxedOperator(const RelaxedOperator&) = default;
    RelaxedOperator(RelaxedOperator&&) noexcept = default;
    RelaxedOperator& operator=(const RelaxedOperator&) = default;
    RelaxedOperator& operator=(RelaxedOperator&&) noexcept = default;

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

bool prune_check(const ResilientNode &node);
PartialState regression(PartialState from, const Operator* op);
bool resiliency_check(const ResilientNode &node);
bool resiliency_check_formula(const ResilientNode &node);
bool replan(const ResilientNode &current_node, SearchEngine *engine);
std::list<Operator> extract_solution(const ResilientNodeFormula &initial_certificate, PartialState goal);
void update_non_resilient_nodes(ResilientNode node);
void add_non_resilient_deadends(ResilientNode node);
Operator* generate_macro_action(PartialState partial_state, int current_level);
std::vector<PartialState> partial_state_to_goal(std::vector<const Operator *> plan);

std::tr1::unordered_map<int, ResilientNode> resilient_nodes;
std::tr1::unordered_map<ResilientNodeFormula, Operator> policy_node;
std::tr1::unordered_map<string, ResilientNodeFormula> macro_to_op;
std::tr1::unordered_map<int, std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>> > resilient_nodes_formula_by_k;
std::tr1::unordered_map<int, ResilientNode> non_resilient_nodes;
std::stack<ResilientNode> open;
bool gen_policy = true;
static std::vector<std::vector<RelaxedProposition> > propositions;
static std::vector<RelaxedOperator> relaxed_operators;
ResilientNodeFormula initial_certificate = ResilientNodeFormula(PartialState(), -1);

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

    g_timer_RCheck.stop();
    g_timer_RCheck.reset();

    g_timer_open_list_generation.stop();
    g_timer_open_list_generation.reset();

    g_time_macro_actions_generation.stop();
    g_time_macro_actions_generation.reset();
    g_timer_check_formula.stop();
    g_timer_check_formula.reset();
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
    g_timer_landmark.stop();
    g_timer_landmark.reset();

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
    PartialState goal_partial_state = PartialState();
    for (int i = 0; i < g_goal.size(); i++){
        (goal_partial_state)[g_goal[i].first] = g_goal[i].second;
    
    }

	for(int i = 0 ; i<= g_max_faults; i++){
    	resilient_nodes_formula_by_k[i] = std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>>();

    }

    // Create initial node and pushing to open stack
    ResilientNode initial_node = ResilientNode(g_initial_state(), g_max_faults, std::set<Operator>());
    open.push(initial_node);
    g_timer_cycle.resume();

    //TODO: il pruning solo per alcuni fatti e landmark, non tutti, non ha senso!
    if(g_pruning){
        g_timer_landmark.resume();
        PartialState current_state = PartialState(initial_node.get_state());
        propositions.clear();
        relaxed_operators.clear();
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
        g_numer_landmarkds = landmarks.size();
        g_max_actions_for_landmark = 0;
        g_min_actions_for_landmark = g_operators_backup.size();
        g_operators = g_operators_backup;
        for (int pos = 0; pos < landmarks.size(); pos++){
            std::pair<int, int> landmark = landmarks[pos];
            int var = landmark.first;
            int value = landmark.second;
            RelaxedProposition &prop = propositions[var][value];
            if(current_state[var] != -1 && current_state[var] != value){
                if(prop.effect_of.size() < g_min_actions_for_landmark)
                    g_min_actions_for_landmark = prop.effect_of.size();
                if(prop.effect_of.size() > g_max_actions_for_landmark)
                    g_max_actions_for_landmark = prop.effect_of.size();
            }
            if (!prop.safe)
            {
                if (current_state[var] != -1 && current_state[var] != value && g_max_faults >= prop.effect_of.size())
                {
                    g_pruning_before_all_value++;
                    open.pop();
                    break;
                }
            }
        }
        g_timer_landmark.stop();
    }
    while (!open.empty()){
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
        if (resilient_nodes.find(current_node.get_id()) == resilient_nodes.end() && non_resilient_nodes.find(current_node.get_id()) == non_resilient_nodes.end())
        {
            g_expanded_node++;
            if (resiliency_check_formula(current_node))
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
                    g_timer_open_list_generation.resume();
                    
                    if (g_verbose)
                        cout << "Successfull replanning" << endl;
                    // Save current initial state in a variable and computed plan for iteration
                    State current = current_node.get_state();
                    std::vector<const Operator *> plan = engine->get_plan();
                    bool prune = false;
                    if (current_node.get_k() >= 1)
                    {
                        for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it)
                        {
                            // Create node <tau_i-1, k, V>
                            ResilientNode res_node = ResilientNode(current, g_current_faults, g_current_forbidden_ops);
                            // Create node <tau_i-1, k - 1, V U {pi_i}>  to compile
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
                                    std::set<Operator> post_actions = g_current_forbidden_ops;
                                    if((*it)->get_name().find("macro_") != std::string::npos && gen_policy){
                              			ResilientNodeFormula tmp = macro_to_op[(*it)->get_name()];
                            			post_actions.insert(tmp.get_next_operator());
                            		}else{
                                    	post_actions.insert(*(*it)); // *it = pi_i
                                    }
                                    ResilientNode res_node_f = ResilientNode(current, g_current_faults - 1, post_actions);
                                    open.push(res_node_f);
                                }
                            }else{
                                open.push(res_node);
                                std::set<Operator> post_actions = g_current_forbidden_ops;
                                post_actions.insert(*(*it)); // *it = pi_i
                                ResilientNode res_node_f = ResilientNode(current, g_current_faults - 1, post_actions);
                                open.push(res_node_f);
                            }
                            current = g_state_registry->get_successor_state(current, *(*it));
                        }
                        if (!prune)
                        {
                            ResilientNode node_tau = ResilientNode(current, g_current_faults, g_current_forbidden_ops);
                            resilient_nodes.insert(make_pair(node_tau.get_id(), node_tau));
                            ResilientNodeFormula tau = ResilientNodeFormula(goal_partial_state, g_current_faults);
                            resilient_nodes_formula_by_k[g_current_faults][tau] = {};
                        }
                    }
                    else {
                        for (vector<const Operator *>::iterator it = plan.begin(); it != plan.end(); ++it){
                            ResilientNode res_node = ResilientNode(current, 0, current_node.get_deactivated_op());
                            resilient_nodes.insert(std::make_pair(res_node.get_id(), res_node));
                            relation_node_next_action[res_node.get_id()] = *(*it);
                            current = g_state_registry->get_successor_state(current, *(*it));
                        }
                        ResilientNode tau = ResilientNode(current, 0, current_node.get_deactivated_op());
                        resilient_nodes.insert(make_pair(tau.get_id(), tau));
                        current = current_node.get_state();
                        set<Operator> formula_pi;
                        PartialState formula = goal_partial_state;
                        ResilientNodeFormula old_res_formula = ResilientNodeFormula(formula, 0);
                        resilient_nodes_formula_by_k[0][old_res_formula] = {};
                        for (size_t i = 0; i < plan.size(); ++i)
                        {
                            Operator* op = const_cast<Operator*>(plan[plan.size()-i-1]);
                            if(op->get_name().find("macro_") != std::string::npos && gen_policy){
                              	ResilientNodeFormula tmp = macro_to_op[op->get_name()];
                                formula_pi.insert(tmp.get_next_operator());
                                set<Operator> tmp_oper_pi = tmp.get_pi();
                                formula_pi.insert(tmp_oper_pi.begin(), tmp_oper_pi.end());
                                formula = regression(formula, op);
                                ResilientNodeFormula res_formula = ResilientNodeFormula(formula, 0, formula_pi, tmp.get_next_operator());
                                if (!resilient_nodes_formula_by_k[0][res_formula].empty()) {
                                    ResilientNodeFormula dummy_1 = resilient_nodes_formula_by_k[0][res_formula][0];
                                    bool security = (res_formula == dummy_1 || res_formula == tmp);
                                    if (!(security)){
                                        resilient_nodes_formula_by_k[0][res_formula] = std::vector<ResilientNodeFormula>{resilient_nodes_formula_by_k[0][tmp]};
                                    }
                                }
                                old_res_formula = res_formula;
                            }else{
                              	formula_pi.insert(*op);
                                formula = regression(formula, op);
								ResilientNodeFormula res_formula = ResilientNodeFormula(formula, 0, formula_pi, *op);
                                if (resilient_nodes_formula_by_k[0][res_formula].empty()) {
                                    resilient_nodes_formula_by_k[0][res_formula] = std::vector<ResilientNodeFormula>{old_res_formula};
                                }
                                old_res_formula = res_formula;
                            }
                        }
                    }
                    g_timer_open_list_generation.stop();
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
        cout << "\nInitial state is resilient, problem is " << g_max_faults << "-resilient!"<< endl;
        if (g_dump_branches)
            print_branches();

        g_timer_extraction.resume();

  	    PartialState initial_state_p = PartialState(static_initial_state);

        std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>> &resilient_nodes_formula_max_faults = resilient_nodes_formula_by_k[g_max_faults];
        for (std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>>::iterator it_2 = resilient_nodes_formula_max_faults.begin(); it_2 != resilient_nodes_formula_max_faults.end(); ++it_2){
        	ResilientNodeFormula current_resilient_node_formula_lower_level = it_2->first;
            if (current_resilient_node_formula_lower_level.get_formula().is_model(initial_state_p)){
	    		initial_certificate = current_resilient_node_formula_lower_level;
	    		continue;
            }
	    }

        print_plan(g_plan_to_file, extract_solution(initial_certificate, goal_partial_state));
        g_timer_extraction.stop();

        g_mem_post_alg = mem_usage();

        if (g_dump_resilient_policy)
        {
            ResilientPolicy res_policy = ResilientPolicy();
            g_timer_extract_policy.resume();
            res_policy.extract_policy(static_initial_state, initial_certificate, goal_partial_state, g_max_faults, resilient_nodes_formula_by_k);
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

Operator* generate_macro_action(PartialState partial_state, int current_level){
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
    Operator* macro = new Operator(formula_previal, pre_post, current_level);
    return macro;
}

bool prune_check(const ResilientNode &node){
    g_timer_landmark.resume();

    PartialState current_state = PartialState(node.get_state());
    g_state_registry->reset_initial_state();
    for (int i = 0; i < g_variable_name.size(); i++)
        g_initial_state_data[i] = current_state[i];
    for (int i = 0; i < g_operators.size(); i++)
    {
        if (g_current_forbidden_ops.find(g_operators[i]) != g_current_forbidden_ops.end())
            g_operators.erase(std::remove_if(g_operators.begin(),g_operators.end(),[&](const Operator &op) { return g_current_forbidden_ops.count(op) > 0;}),g_operators.end());
    }
    propositions.clear();
    relaxed_operators.clear();
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
                return true;
            }
        }
    }
    g_timer_landmark.stop();
    return false;
}


bool resiliency_check_formula(const ResilientNode &node)
{
    g_timer_RCheck.resume();
    if (resilient_nodes_formula_by_k[node.get_k()].empty())
    {
        g_timer_RCheck.stop();
        return false;
    }
    State state = node.get_state();
    PartialState current_r = PartialState(state);
    vector< Operator> stored_ops;
    vector<const Operator *> all_operators;
    list<PolicyItem *> current_policy = g_policy->get_items();


    // Find from the policy every action applicable in the current state, minus V
    for (std::list<PolicyItem *>::iterator it = current_policy.begin(); it != current_policy.end(); ++it)
    {
        RegressionStep *reg_step = dynamic_cast<RegressionStep *>(*it);
        if (!reg_step->is_goal)
        {
            PartialState policy_state = PartialState(*reg_step->state);
            if ((*reg_step->state).is_implied(current_r) && !find_in_op_set(node.get_deactivated_op(), reg_step->get_op())){
                Operator curr_op = reg_step->get_op();
                if (curr_op.get_name().find("macro_") != std::string::npos && gen_policy)
                {
                    ResilientNodeFormula tmp = macro_to_op[curr_op.get_name()];
                    Operator new_op = tmp.get_next_operator();
                    stored_ops.push_back(new_op);
                }else
                {
                    stored_ops.push_back(curr_op);                

                }
            }
        }
    }

    for (auto& oper : stored_ops) {     
        all_operators.push_back(&oper);
    }


    StateRegistry *registry = const_cast<StateRegistry *>(&state.get_registry());

    std::set<Operator> current_node_forbidden = node.get_deactivated_op();
    std::set<Operator> forbidden_plus_current = node.get_deactivated_op();
	std::vector<const Operator*> filtered_operators;
	for (const Operator* op_ptr : all_operators) {
    	if (current_node_forbidden.find(*op_ptr) == current_node_forbidden.end()) {
            filtered_operators.push_back(op_ptr);
    	}
	}
    for(auto ope : filtered_operators){
        forbidden_plus_current = node.get_deactivated_op();
        forbidden_plus_current.insert(*ope);
        State successor = registry->get_successor_state(state, *ope);
        PartialState successor_p = PartialState(successor);
        if(node.get_k() == 0){
            for (int i = 0; i <= g_max_faults; i++){
                auto& resilient_nodes_formula = resilient_nodes_formula_by_k[i];
                for (std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>>::iterator it = resilient_nodes_formula.begin(); it != resilient_nodes_formula.end(); ++it){
                    ResilientNodeFormula successor_node_formula_same_level_k = it->first;
                    bool invalid_successor_same_level = false;
                    if (successor_node_formula_same_level_k.get_formula().is_model(successor_p) && successor_node_formula_same_level_k.get_k() == node.get_k())
            		{
                        set<Operator> same_level_pi = successor_node_formula_same_level_k.get_pi();
                    	for (std::set<Operator>::iterator it_current_forbidden_operator = current_node_forbidden.begin(); it_current_forbidden_operator != current_node_forbidden.end(); ++it_current_forbidden_operator)
                    	{
                    	for (std::set<Operator>::iterator it_same_level_pi_operator = same_level_pi.begin(); it_same_level_pi_operator != same_level_pi.end(); ++it_same_level_pi_operator)
                        	{
                            	if ((*it_same_level_pi_operator) == (*it_current_forbidden_operator)) {
                                    invalid_successor_same_level = true;
                                }
                            }
                    	}
                        if (!invalid_successor_same_level) {
                        	set<Operator> pi_equal_next_formula = successor_node_formula_same_level_k.get_pi();
                            pi_equal_next_formula.insert(*ope);
                            PartialState formula = regression(successor_node_formula_same_level_k.get_formula(), ope);
                            ResilientNodeFormula to_add = ResilientNodeFormula(formula, node.get_k(), pi_equal_next_formula, *ope);
                            if (resilient_nodes_formula_by_k[node.get_k()][to_add].empty()) {
                                resilient_nodes_formula_by_k[node.get_k()][to_add] = std::vector<ResilientNodeFormula>{successor_node_formula_same_level_k};
                            }
                            g_timer_check_formula.stop();
                            g_timer_RCheck.stop();
                            return true;
                        }
                    }
                }
            }
        }
        else{
            for (int i = node.get_k(); i <= g_max_faults; i++){
                std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>>& resilient_nodes_formula = resilient_nodes_formula_by_k[i];
                for (std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>>::iterator it = resilient_nodes_formula.begin(); it != resilient_nodes_formula.end(); ++it){
                    ResilientNodeFormula successor_node_formula_same_level_k = it->first;
                    bool invalid_successor_same_level = false;
                    set<Operator> same_level_pi = successor_node_formula_same_level_k.get_pi();
                    if (successor_node_formula_same_level_k.get_formula().is_model(successor_p) && successor_node_formula_same_level_k.get_k() == node.get_k())
                    {
                    for (std::set<Operator>::iterator it_current_forbidden_operator = current_node_forbidden.begin(); it_current_forbidden_operator != current_node_forbidden.end(); ++it_current_forbidden_operator)
                    {
                    	for (std::set<Operator>::iterator it_same_level_pi_operator = same_level_pi.begin(); it_same_level_pi_operator != same_level_pi.end(); ++it_same_level_pi_operator)
                        {
                            if ((*it_same_level_pi_operator) == (*it_current_forbidden_operator)) {
                                invalid_successor_same_level = true;
                            }
                        }
                    }
                    if(!invalid_successor_same_level){
                        for (int y = node.get_k() -1 ; y <= g_max_faults; y++){
                            auto& resilient_nodes_formula_y = resilient_nodes_formula_by_k[y];
                            for (std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>>::iterator it_2 = resilient_nodes_formula_y.begin(); it_2 != resilient_nodes_formula_y.end(); ++it_2){
                              	ResilientNodeFormula current_resilient_node_formula_lower_level = it_2->first;
                            	bool current_lower_level_is_invalid = false;
                                set<Operator> lower_level_pi = current_resilient_node_formula_lower_level.get_pi();
                                if (current_resilient_node_formula_lower_level.get_formula().is_model(current_r) && (current_resilient_node_formula_lower_level.get_k() == (node.get_k() - 1))){
                                	for (std::set<Operator>::iterator it_current_forbidden_plus_current_operator = forbidden_plus_current.begin(); it_current_forbidden_plus_current_operator != forbidden_plus_current.end(); ++it_current_forbidden_plus_current_operator){
                            			for (std::set<Operator>::iterator it_lower_level_pi_operator = lower_level_pi.begin(); it_lower_level_pi_operator != lower_level_pi.end(); ++it_lower_level_pi_operator){
                                    		if ((*it_current_forbidden_plus_current_operator) == (*it_lower_level_pi_operator)) {
                                	    		current_lower_level_is_invalid = true;
                                            }
                                        }
                            		}
                                    if(!current_lower_level_is_invalid){
                                        set<Operator> pi_equal_next_formula;
                                        set<Operator> pi_lower_same_formula = current_resilient_node_formula_lower_level.get_pi();
    			    	    			PartialState formula;
                                        pi_equal_next_formula = successor_node_formula_same_level_k.get_pi();
                                        pi_equal_next_formula.insert(pi_lower_same_formula.begin(), pi_lower_same_formula.end());
                                        pi_equal_next_formula.insert(successor_node_formula_same_level_k.get_next_operator());
                                        pi_equal_next_formula.insert(*ope);
                                        formula = successor_node_formula_same_level_k.get_formula();
                                        formula = regression(successor_node_formula_same_level_k.get_formula(), ope);
                                        formula.combine_with(current_resilient_node_formula_lower_level.get_formula());
                                        ResilientNodeFormula to_add = ResilientNodeFormula(formula, node.get_k(), pi_equal_next_formula, *ope);
                                        if (resilient_nodes_formula_by_k[node.get_k()][to_add].empty()) {
                                            resilient_nodes_formula_by_k[node.get_k()][to_add] = std::vector<ResilientNodeFormula>{successor_node_formula_same_level_k, current_resilient_node_formula_lower_level};
                                        }
                                        g_timer_RCheck.stop();
                                        return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    g_timer_RCheck.stop();
    return false;
}


bool resiliency_check(const ResilientNode &node)
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
    }
    return partial_states;
}

bool replan(const ResilientNode &current_node, SearchEngine *engine){
    PartialState current_state = PartialState(current_node.get_state());
    g_macro_actions.clear();
    g_state_registry->reset_initial_state();
    for (int i = 0; i < g_variable_name.size(); i++)
        g_initial_state_data[i] = current_state[i];
    for (int i = 0; i < g_operators.size(); i++){
        if (g_current_forbidden_ops.find(g_operators[i]) != g_current_forbidden_ops.end())
            g_operators.erase(g_operators.begin() + i--);
    }
    if(g_use_macro_actions){
        g_time_macro_actions_generation.resume();
        PartialState goal_partial_state = PartialState();
        for (int i = 0; i < g_goal.size(); i++)
        {
            (goal_partial_state)[g_goal[i].first] = g_goal[i].second;
        }
        // gia cilare sulla policy
        for (int k = current_node.get_k(); k <= g_max_faults; k++)
        {
            std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>>& resilient_nodes_formula = resilient_nodes_formula_by_k[k];
            for (std::tr1::unordered_map<ResilientNodeFormula, std::vector<ResilientNodeFormula>>::iterator it = resilient_nodes_formula.begin(); it != resilient_nodes_formula.end(); ++it)
            {
                ResilientNodeFormula node_formula = it->first;
                if (!node_formula.get_formula().is_model(goal_partial_state))
                {
                    if (current_node.get_k() == node_formula.get_k())
                    {
                        set<Operator> pi = node_formula.get_pi();
                        set<Operator> current_forbidden = current_node.get_deactivated_op();
                        bool use_macro = true;
                        for (std::set<Operator>::iterator it_current_forbidden_operator = current_forbidden.begin(); it_current_forbidden_operator != current_forbidden.end(); ++it_current_forbidden_operator)
                        {
                            for (std::set<Operator>::iterator it_pi_operator = pi.begin(); it_pi_operator != pi.end(); ++it_pi_operator)
                            {
                                if ((*it_current_forbidden_operator) == (*it_pi_operator))
                                    use_macro = false;
                            }
                        }
                        if (use_macro)
                        {
                            Operator *macro = generate_macro_action(node_formula.get_formula(), current_node.get_k());
                            g_macro_actions.push_back(macro);
                            macro_to_op[macro->get_name()] = node_formula;
                        }
                    }
                }
            }
        }
        g_time_macro_actions_generation.stop();
    }
   
    g_timer_engine_init.resume();
    engine->reset();
    g_timer_engine_init.stop();


    g_timer_search.resume();
    engine->search();

    g_timer_search.stop();
    g_macro_actions.clear();
    if (g_dump_memory_replan_progression)
        cout << "Memory at repla #" << g_replan_counter + 1 << ": " << mem_usage() << "KB" << endl;
    return engine->found_solution();
}

/// @brief Extract the final resilient plan, starting by the initial state
/// and choosing only actions that lead to resilient states until the goal
/// is reached.
/// @return The plan extracted.

std::list<Operator> extract_solution(const ResilientNodeFormula &initial_certificate, PartialState goal)
{

  	std::list<Operator> plan;
    ResilientNodeFormula current_certificate = initial_certificate;
    while (!current_certificate.get_formula().is_model(goal))
    {
		plan.push_back(current_certificate.get_next_operator());
        if(resilient_nodes_formula_by_k[g_max_faults][current_certificate][0].get_k() == g_max_faults){
        	current_certificate = resilient_nodes_formula_by_k[g_max_faults][current_certificate][0];
        }else{
          	current_certificate = resilient_nodes_formula_by_k[g_max_faults][current_certificate][1];
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