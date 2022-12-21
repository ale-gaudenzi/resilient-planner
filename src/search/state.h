#ifndef STATE_H
#define STATE_H

#include "int_packer.h"
#include "state_id.h"
//#include "operator.h"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

//class Operator;
class StateRegistry;

typedef IntPacker::Bin PackedStateBin;

class StateInterface {
public:
    virtual int operator[](int index) const = 0;
    virtual ~StateInterface() {};
};

// For documentation on classes relevant to storing and working with registered
// states see the file state_registry.h.
class State : public StateInterface {
    friend class StateRegistry;
    template <class Entry>
    friend class PerStateInformation;

protected:
    // Values for vars are maintained in a packed state and accessed on demand.
    const PackedStateBin *buffer;

    // registry isn't a reference because we want to support operator=
    const StateRegistry *registry;
    StateID id;

    // Only used by the state registry.
    State(const PackedStateBin *buffer_, const StateRegistry &registry_,
          StateID id_);

    const PackedStateBin *get_packed_buffer() const {
        return buffer;
    }

    const StateRegistry &get_registry() const {
        return *registry;
    }

    // No implementation to prevent default construction
    State();
public:
    ~State();

    StateID get_id() const {
        return id;
    }

    int operator[](int index) const;

    void dump_pddl() const;
    void dump_fdr() const;
};

/*
class ResilientState : public State {
    friend class StateRegistry;
    int k;
    std::set<Operator> forbidden_op;

    ResilientState();

    ResilientState(const PackedStateBin *buffer_, const StateRegistry &registry_,
                   StateID id_, int k_, std::set<Operator> forbidden_op_);

    ResilientState(const State &state_, int k_);
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
*/




#endif
