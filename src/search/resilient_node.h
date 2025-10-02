#ifndef RESILIENT_NODE_H
#define RESILIENT_NODE_H

#include "operator.h"
#include <tr1/unordered_map>
#include <set>
#include "state.h"
#include <functional>
#include <cstddef> // size_t

// Evita: using namespace std;

// Assumo: State è indicizzabile come vector<int> (adatta se diverso)
struct NodeKey {
    State state;                  // es. std::vector<int>
    int k;
    std::set<Operator> deactivated_op;  // ordine deterministico

    bool operator==(const NodeKey& other) const {
        return k == other.k &&
               state == other.state &&
               deactivated_op == other.deactivated_op;
    }
};

// hash_combine stile Boost
inline void hash_combine(std::size_t& seed, std::size_t v) {
    seed ^= v + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

struct NodeKeyHash {
    std::size_t operator()(const NodeKey& nk) const {
        std::size_t h = 0;
        for (auto &op : nk.deactivated_op)
            hash_combine(h, std::tr1::hash<std::string>{}(op.get_name()));
        for (std::size_t i=0; i<g_variable_domain.size(); ++i)
            hash_combine(h, std::tr1::hash<int>{}(nk.state[i]));
        hash_combine(h, std::tr1::hash<int>{}(nk.k));
        return h;
    }
};

// DICHIARAZIONE (non definizione) della tabella globale
extern tr1::unordered_map<NodeKey,int,NodeKeyHash> key_to_id;
extern int next_id;

class ResilientNode {
    State state;
    int k;
    std::set<Operator> deactivated_op;
    int id;
    ResilientNode();

public:
    ResilientNode(State state_, int k_, std::set<Operator> deactivated_op_);

    int get_id() const { return id; }
    State get_state() const { return state; }
    int get_k() const { return k; }
    void dump() const;
    std::set<Operator> get_deactivated_op() const { return deactivated_op; }

    bool operator==(const ResilientNode& other) const { return id == other.id; }
    bool operator!=(const ResilientNode& other) const { return id != other.id; }
    bool operator<(const ResilientNode& other) const { return id < other.id; }
};

#endif