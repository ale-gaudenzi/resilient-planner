#include "resilient_state.h"
#include "globals.h"
#include "utilities.h"


using namespace std;


ResilientState::ResilientState(const PartialState &partial_state_, int k_, std::set<Operator> deactivated_op_) 
                : PartialState(partial_state_) {
    k = k_;
    deactivated_op = deactivated_op_;
    is_resilient = true;
}

ResilientState::ResilientState(const PartialState &partial_state_) : PartialState(partial_state_) {
    k = 0;
    std::set<Operator> empty;
    deactivated_op = empty;
    is_resilient = true;
}

void ResilientState::dump() const {
    cout << "resilient_state: " << this << endl;
    this->dump_pddl();
    cout << "k: " << k << endl;
    cout << "deactivated_op: " << endl;
    for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++) {
        it->dump();
    }
    cout << "\n";
}