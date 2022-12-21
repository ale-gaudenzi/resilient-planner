#include "resilient_state.h"
#include "globals.h"
#include "utilities.h"


using namespace std;


ResilientState::ResilientState(const PartialState &partial_state_, int k_, std::set<Operator> forbidden_op_) 
                : PartialState(partial_state_) {
    k = k_;
    forbidden_op = forbidden_op_;
}

