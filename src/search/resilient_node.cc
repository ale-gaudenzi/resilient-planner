#include "resilient_node.h"
#include <utility>   // std::move
#include <tr1/unordered_map>
#include <mutex>     // se ti serve thread-safety


using namespace std;

/// @brief Constructor for ResilientNode that takes all the parameters needed to create a node
/// @param state_ Full state of the node
/// @param k_ Number of operators that can still fail
/// @param deactivated_op_ Deactivated opera tors

// Tabella di internamento (una sola definizione qui)

std::tr1::unordered_map<NodeKey,int,NodeKeyHash> key_to_id;
int next_id = 1;

ResilientNode::ResilientNode(State state_, int k_, std::set<Operator> deactivated_op_): state(state_), k(k_), deactivated_op(deactivated_op_), id(0)
{
   NodeKey key{state, k, deactivated_op};

    std::tr1::unordered_map<NodeKey,int,NodeKeyHash>::iterator it = key_to_id.find(key);
    if (it != key_to_id.end()) {
        id = it->second;   // già presente
    } else {
        id = next_id++;
        key_to_id.insert(std::make_pair(key, id)); // non c’è emplace in TR1
    }
}

void ResilientNode::dump() const
{
    cout << "Node: " << id << endl;
    state.dump_pddl();
    cout << "k: " << k << endl;
    cout << "deactivated_op: " << endl;
    for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
        cout << it->get_nondet_name() << endl;
}
