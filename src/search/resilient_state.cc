#include "resilient_state.h"
#include "globals.h"
#include "utilities.h"


using namespace std;


ResilientState::ResilientState(const PartialState &partial_state_, int k_, std::set<Operator> forbidden_op_) 
                : PartialState(partial_state_) {
    k = k_;
    forbidden_op = forbidden_op_;
}

ResilientState::ResilientState(const PartialState &partial_state_) : PartialState(partial_state_) {
    k = 0;
    std::set<Operator> empty;
    forbidden_op = empty;
}

void ResilientState::dump() const {
    cout << "resilient_state: " << this << endl;
    this->dump_pddl();
    cout << "k: " << k << endl;
    cout << "forbidden_op: " << endl;
    for (set<Operator>::iterator it = forbidden_op.begin(); it != forbidden_op.end(); it++) {
        it->dump();
    }
    cout << "\n";
}