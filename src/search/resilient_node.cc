#include "resilient_node.h"

using namespace std;

/// @brief Constructor for ResilientNode that takes all the parameters needed to create a node
/// @param state_ Full state of the node
/// @param k_ Number of operators that can still fail
/// @param deactivated_op_ Deactivated operators
ResilientNode::ResilientNode(State state_, int k_, std::set<Operator> deactivated_op_) : state(state_), k(k_), deactivated_op(deactivated_op_)
{
    int op_value;

    if (deactivated_op.size() != 0)
        for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
            op_value += it->nondet_index;
    else
        op_value = 0;

    id = (unsigned long int)state.get_packed_buffer() + k + op_value;
}

/// @brief Constructor for ResilientNode with empty set of deactivated operators, used for first node
/// @param state_ Full state of the node
/// @param k_ Number of operators that can still fail
ResilientNode::ResilientNode(State state_, int k_) : state(state_), k(k_)
{
    std::set<Operator> empty;
    deactivated_op = empty;

    id = (unsigned long int)state.get_packed_buffer() + k;
}

/// @brief Constructor for ResilientNode with empty set of deactivated operators and k = 0
/// @param state_ Full state of the node
ResilientNode::ResilientNode(State state_) : state(state_)
{
    k = 0;
    std::set<Operator> empty;
    deactivated_op = empty;

    id = (unsigned long int)state.get_packed_buffer();
}

/// @brief Dump node information to the standard output
void ResilientNode::dump() const
{
    cout << "Node: " << id << endl;
    state.dump_pddl();
    cout << "k: " << k << endl;
    cout << "deactivated_op: " << endl;
    for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
        cout << it->get_nondet_name() << endl;
}
/*
/// @brief Custom equality operator
/// @param other the other node to compare to
/// @return true if the nodes are equal, false otherwise
bool ResilientNode::operator==(const ResilientNode &other) const
{
    bool equal_op = true;

    if (deactivated_op.size() != other.deactivated_op.size())
        return false;

    else if (deactivated_op.size() != 0)
        for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
            if (other.deactivated_op.find(*it) == other.deactivated_op.end())
                equal_op = false;

    return (state.get_packed_buffer() == other.state.get_packed_buffer()) && (k == other.k) && equal_op;
}

/// @brief Custom minority operator
/// @param other the other node to compare to
/// @return true if other node is minor that this, false otherwise
bool ResilientNode::operator<(const ResilientNode &other) const
{
    int op_value;
    int other_op_value;

    if (deactivated_op.size() != 0)
        for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
            op_value += it->nondet_index;
    else
        op_value = 0;

    if (other.deactivated_op.size() != 0)
        for (set<Operator>::iterator it = other.deactivated_op.begin(); it != other.deactivated_op.end(); it++)
            other_op_value += it->nondet_index;
    else
        other_op_value = 0;

    long int value = (unsigned long int)state.get_packed_buffer() + k + op_value;
    long int other_value = (unsigned long int)other.state.get_packed_buffer() + other.k + other_op_value;

    return value < other_value;
}

bool ResilientNode::operator!=(const ResilientNode &other) const
{
    int op_value;
    int other_op_value;

    if (deactivated_op.size() != 0)
        for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
            op_value += it->nondet_index;
    else
        op_value = 0;

    if (other.deactivated_op.size() != 0)
        for (set<Operator>::iterator it = other.deactivated_op.begin(); it != other.deactivated_op.end(); it++)
            other_op_value += it->nondet_index;
    else
        other_op_value = 0;

    long int value = (unsigned long int)state.get_packed_buffer() + k + op_value;
    long int other_value = (unsigned long int)other.state.get_packed_buffer() + other.k + other_op_value;

    return value != other_value;
}
*/
