#include "resilient_state.h"

#include "globals.h"
#include "utilities.h"

#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;


ResilientState::ResilientState(const State &state_, int k_, std::set<Operator> forbidden_op_) 
                : State(state_) {
    k = k_;
    forbidden_op = forbidden_op_;
}

