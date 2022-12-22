#ifndef RESILIENT_STATE_H
#define RESILIENT_STATE_H

#include "operator.h"
#include "policy-repair/partial_state.h"

#include <set>

using namespace std;

typedef IntPacker::Bin PackedStateBin;

class ResilientState : public PartialState {
    friend class StateRegistry;
    int k;
    
    std::set<Operator> deactivated_op;

    ResilientState();


public:
    ResilientState(const PartialState &partial_state_, int k_, std::set<Operator> deactivated_op_);
    ResilientState(const PartialState &partial_state_);
    
    void dump() const;

    int get_k() const {
        return k;
    }

    std::set<Operator> get_deactivated_op() const {
        return deactivated_op;
    }
};

#endif
