#ifndef RESILIENT_STATE_H
#define RESILIENT_STATE_H

#include "int_packer.h"
#include "state_id.h"
#include "state.h"
#include "state_registry.h"
#include "operator.h"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

typedef IntPacker::Bin PackedStateBin;

class ResilientState : public State {
    friend class StateRegistry;
    int k;
    std::set<Operator> forbidden_op;

    ResilientState();

    ResilientState(const State &state_, int k_, std::set<Operator> forbidden_op_);

public:

    int get_k() const
    {
        return k;
    }

    std::set<Operator> get_forbidden_op() const {
        return forbidden_op;
    }
};

#endif
