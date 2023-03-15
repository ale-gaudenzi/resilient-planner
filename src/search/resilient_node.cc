#include "resilient_node.h"

using namespace std;

ResilientNode::ResilientNode(State state_, int k_, std::set<Operator> deactivated_op_) : state(state_), k(k_), deactivated_op(deactivated_op_) {
}

ResilientNode::ResilientNode(State state_, int k_) : state(state_), k(k_) {
    std::set<Operator> empty;
    deactivated_op = empty;
}

ResilientNode::ResilientNode(State state_) : state(state_) {
    k = 0;
    std::set<Operator> empty;
    deactivated_op = empty;
}

void ResilientNode::dump() const {
    cout << "Node: " << this << endl;
    state.dump_pddl();
    cout << "k: " << k << endl;
    cout << "deactivated_op: " << endl;
    for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++) {
        it->dump();
    }
}


bool ResilientNode::operator==(const ResilientNode &other) const {
    bool equal_op = true;
    
    if (deactivated_op.size() != other.deactivated_op.size()){
        cout << "KKKKKKKKKKKKK FALSO" << endl;
        return false;
    }

    else if (deactivated_op.size() != 0) {
        for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++) {
            if (other.deactivated_op.find(*it) == other.deactivated_op.end()) {
                equal_op = false;
            }
        }
    }   
    cout << "KKKKKKKKKKKKKKK equal_op: " << equal_op << endl;

    return (state.get_packed_buffer() == other.state.get_packed_buffer()) && (k == other.k) && equal_op; //&& (deactivated_op == other.deactivated_op);
}

bool ResilientNode::operator<(const ResilientNode &other) const {
    return (state.get_id().get_value() < other.state.get_id().get_value()) && (k < other.k) && (deactivated_op < other.deactivated_op);
}




