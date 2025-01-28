#ifndef RESILIENT_NODE_FORMULA_H
#define RESILIENT_NODE_FORMULA_H

#include "operator.h"
#include <set>

using namespace std;

class ResilientNodeFormula
{
    PartialState formula;
    int k;
    std::set<Operator> deactivated_op;
    long int id;
    ResilientNodeFormula();

public:
    ResilientNodeFormula(PartialState formula, int k_, std::set<Operator> deactivated_op_);

    void dump() const;

    PartialState get_formula() const
    {
        return formula;
    }

    int get_k() const
    {
        return k;
    }

    int get_id() const
    {
        return id;
    }

    std::set<Operator> get_deactivated_op() const
    {
        return deactivated_op;
    }

    bool operator==(const ResilientNodeFormula &other) const
    {
        return this->id == other.id;
    }
    bool operator<(const ResilientNodeFormula &other) const
    {
        return this->id < other.id;
    }
    bool operator!=(const ResilientNodeFormula &other) const
    {
        return this->id != other.id;
    }
};

#endif
