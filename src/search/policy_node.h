#ifndef POLICY_NODE_H
#define POLICY_NODE_H

#include "operator.h"
#include "resilient_node_formula.h"
#include "policy_node.h"

using namespace std;

class PolicyNode
{
    ResilientNodeFormula resilient_node_formula;
    Operator op;
    long int id;

public:
    PolicyNode();
    PolicyNode(ResilientNodeFormula resilient_node_formula_, Operator op_);

    void dump() const;

    ResilientNodeFormula get_resilient_node_formula() const
    {
        return resilient_node_formula;
    }

    long int get_id() const { return id; };

    Operator get_operator() const
    {
        return op;
    }

    bool operator==(const PolicyNode &other) const
    {
        return this->id == other.id;
    }
    bool operator<(const PolicyNode &other) const
    {
        return this->id < other.id;
    }
    bool operator!=(const PolicyNode &other) const
    {
        return this->id != other.id;
    }
};

#endif
