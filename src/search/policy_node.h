#ifndef POLICY_NODE_H
#define POLICY_NODE_H

#include "operator.h"
#include "resilient_node_formula.h"
#include "policy_node.h"

using namespace std;

class PolicyNode
{
    long int id;
    ResilientNodeFormula certificate;
    int k;
    set<Operator> forbidden_actions;
    int current_level_resiliency;
    int lower_level_resiliency;

public:
    PolicyNode(ResilientNodeFormula certificate_, int k_, set<Operator> forbidden_actions_, int current_level_resiliency_, int lower_level_resiliency_ );
    PolicyNode() = default;

    int get_id() const { return id; };

    ResilientNodeFormula get_certificate() const { return certificate; };

    set<Operator> get_forbidden_actions() const { return forbidden_actions; };

    int get_k() const { return k; };

    int get_lower_level_resiliency_id() const {return lower_level_resiliency;}

    int get_current_level_resiliency_id() const{return current_level_resiliency;}

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
