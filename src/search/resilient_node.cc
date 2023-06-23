#include "resilient_node.h"

using namespace std;

/// @brief Constructor for ResilientNode that takes all the parameters needed to create a node
/// @param state_ Full state of the node
/// @param k_ Number of operators that can still fail
/// @param deactivated_op_ Deactivated operators
ResilientNode::ResilientNode(State state_, int k_, std::set<Operator> deactivated_op_) : state(state_), k(k_), deactivated_op(deactivated_op_)
{
}

/// @brief Constructor for ResilientNode with empty set of deactivated operators, used for first node
/// @param state_ Full state of the node
/// @param k_ Number of operators that can still fail
ResilientNode::ResilientNode(State state_, int k_) : state(state_), k(k_)
{
    std::set<Operator> empty;
    deactivated_op = empty;
}

/// @brief Constructor for ResilientNode with empty set of deactivated operators and k = 0
/// @param state_ Full state of the node
ResilientNode::ResilientNode(State state_) : state(state_)
{
    k = 0;
    std::set<Operator> empty;
    deactivated_op = empty;
}

/// @brief Dump node information to the standard output
void ResilientNode::dump() const
{
    cout << "Node: " << this << endl;
    state.dump_pddl();
    cout << "k: " << k << endl;
    cout << "deactivated_op: " << endl;
    for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
    {
        it->dump();
    }
}

/// @brief Custom equality operator
/// @param other the other node to compare to
/// @return true if the nodes are equal, false otherwise
bool ResilientNode::operator==(const ResilientNode &other) const
{
    bool equal_op = true;

    if (deactivated_op.size() != other.deactivated_op.size())
    {
        return false;
    }

    else if (deactivated_op.size() != 0)
    {
        for (set<Operator>::iterator it = deactivated_op.begin(); it != deactivated_op.end(); it++)
        {
            if (other.deactivated_op.find(*it) == other.deactivated_op.end())
            {
                equal_op = false;
            }
        }
    }

    return (state.get_packed_buffer() == other.state.get_packed_buffer()) && (k == other.k) && equal_op; //&& (deactivated_op == other.deactivated_op);
}

/// @brief Custom minority operator
/// @param other the other node to compare to
/// @return true if other node is minor that this, false otherwise
bool ResilientNode::operator<(const ResilientNode &other) const
{
    return this < &other;
}
