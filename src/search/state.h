#ifndef STATE_H
#define STATE_H

#include "int_packer.h"
#include "state_id.h"
#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

class StateRegistry;

typedef IntPacker::Bin PackedStateBin;

class StateInterface
{
public:
    virtual int operator[](int index) const = 0;
    virtual ~StateInterface(){};
};

// For documentation on classes relevant to storing and working with registered
// states see the file state_registry.h.
class State : public StateInterface
{
    friend class StateRegistry;
    template <class Entry>
    friend class PerStateInformation;
    friend class ResilientNode;

protected:
    // Values for vars are maintained in a packed state and accessed on demand.
    const PackedStateBin *buffer;

    // registry isn't a reference because we want to support operator=
    const StateRegistry *registry;
    StateID id;

    // Only used by the state registry.
    State(const PackedStateBin *buffer_, const StateRegistry &registry_,
          StateID id_);

    const PackedStateBin *get_packed_buffer() const
    {
        return buffer;
    }

    // No implementation to prevent default construction

public:
    State();

    ~State();

    StateID get_id() const
    {
        return id;
    }

    const StateRegistry &get_registry() const
    {
        return *registry;
    }

    int operator[](int index) const;
    bool operator==(const State &other) const;

    void dump_pddl() const;
    void dump_fdr() const;
};

#endif
