#include "globals.h"
#include "operator.h"

#include <string>
#include <iostream>
#include <tr1/unordered_map>
using std::tr1::hash;




using namespace std;


Prevail::Prevail(istream &in) {
    in >> var >> prev;
}

PrePost::PrePost(istream &in) {
    int condCount;
    in >> condCount;
    for (int i = 0; i < condCount; i++)
        cond.push_back(Prevail(in));
    in >> var >> pre >> post;
}



Operator::Operator(std::vector<Prevail> prevail_, std::vector<PrePost> pre_post_, int k){
    nondet_index = 0;
    prevail = prevail_;
    pre_post = pre_post_;
    string to_hash = "";
    std::tr1::hash<string> hasher;
    for (std::vector<Prevail>::const_iterator it = prevail.begin(); it != prevail.end(); ++it) {
        const Prevail pre = *it;
        to_hash += g_fact_names[pre.var][pre.prev];
        to_hash += "_";
    }
    for (std::vector<PrePost>::const_iterator it = pre_post.begin(); it != pre_post.end(); ++it) {
        const PrePost post = *it;
        if (-1 == post.pre)
            to_hash += "anything_";
        else {
            to_hash += g_fact_names[post.var][post.pre] + "_"  + g_fact_names[post.var][post.post] + "_";
        }
    }

    is_an_axiom = false;
    safe = false;
    cost = 0;

    std::ostringstream oss;
    oss << k;
    std::string k_str = oss.str();
    int hash = hasher(to_hash + "-" + k_str);

    std::ostringstream oss_;
    oss_ << hash;
    std::string hash_str = oss_.str();

    name = "macro_" + hash_str;
    nondet_name = name;
}

Operator::Operator() {
    marked = false;
    nondet_index = -1;
    is_an_axiom = false;
    name = "dummy__action";
    cost = -1;
}

Operator::Operator(istream &in, bool axiom) {
    marked = false;
    nondet_index = -1;

    is_an_axiom = axiom;
    if (!is_an_axiom) {
        check_magic(in, "begin_operator");
        in >> ws;
        getline(in, name);
        int count;
        in >> count;
        for (int i = 0; i < count; i++)
            prevail.push_back(Prevail(in));
        in >> count;
        for (int i = 0; i < count; i++)
            pre_post.push_back(PrePost(in));

        int op_cost;
        in >> op_cost;
        cost = g_use_metric ? op_cost : 1;

        g_min_action_cost = min(g_min_action_cost, cost);
        g_max_action_cost = max(g_max_action_cost, cost);

        check_magic(in, "end_operator");
        
        // The nondet name is the original name of the non-deterministic action
        if (name.find("_DETDUP") == string::npos) {
            nondet_name = name;
            safe = false;
        }
        else
        {
            nondet_name = name.substr(0, name.find("_DETDUP")) + name.substr(name.find(" "), string::npos);
            safe = false;
        }
    } else {
        name = "<axiom>";
        cost = 0;
        check_magic(in, "begin_rule");
        pre_post.push_back(PrePost(in));
        check_magic(in, "end_rule");
    }

    marker1 = marker2 = false;
    
    
    
    /* ********************
     * 
     *  !!WARNING!!
     * 
     *   This operation doesn't check for inconsistencies
     *    because we assume that was done to construct the
     *    g_regressable_cond_ops data structure (see the
     *    generate_regressable_ops function).
     * 
     * ************* */
     
    // Deal with the all-fire context (essentially every conditional head
    //  and precondition rolled into one state)
    all_fire_context = new PartialState();
    
    for (int i = 0; i < pre_post.size(); i++) {
        (*all_fire_context)[pre_post[i].var] = pre_post[i].pre;
        
        for (int j = 0; j < pre_post[i].cond.size(); j++) {
            (*all_fire_context)[pre_post[i].cond[j].var] = pre_post[i].cond[j].prev;
        }
    }
    
    for (int i = 0; i < prevail.size(); i++)
        (*all_fire_context)[prevail[i].var] = prevail[i].prev;
    
    
}

void Prevail::dump() const {
    const string &fact_name = g_fact_names[var][prev];
    if (fact_name != "<none of those>")
        cout << fact_name;
    else
        cout << "[" << g_variable_name[var] << "] None of those.";
}

void PrePost::dump() const {
    
    if (-1 == pre)
        cout << "anything -> ";
    else {
        const string &pre_name = g_fact_names[var][pre];
        if (pre_name != "<none of those>")
            cout << pre_name << " -> ";
        else
            cout << "[" << g_variable_name[var] << "] None of those." << " -> ";
    }
    
    const string &post_name = g_fact_names[var][post];
    if (post_name != "<none of those>")
        cout << post_name;
    else
        cout << "[" << g_variable_name[var] << "] None of those.";
    if (!cond.empty()) {
        cout << " if";
        for (int i = 0; i < cond.size(); i++) {
            cout << " ";
            cond[i].dump();
        }
    }
}

void Operator::dump() const {
    cout << name << " (" << nondet_name << "):";
    for (int i = 0; i < prevail.size(); i++) {
        cout << " [";
        prevail[i].dump();
        cout << "]";
    }
    for (int i = 0; i < pre_post.size(); i++) {
        cout << " [";
        pre_post[i].dump();
        cout << "]";
    }
    cout << endl;
}
