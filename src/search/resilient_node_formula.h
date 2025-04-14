#ifndef RESILIENT_NODE_FORMULA_H
#define RESILIENT_NODE_FORMULA_H

#include "operator.h"
#include <set>


using namespace std;

class ResilientNodeFormula
{
    PartialState formula;
    int k;
    std::set<Operator> pi;
    long int id;

public:
    ResilientNodeFormula(PartialState formula, int k_, std::set<Operator> pi_);
    ResilientNodeFormula(PartialState formula, int k_);

    void dump() const;

    PartialState get_formula() const
    {
        return formula;
    }

    set<Operator> get_pi(){
        return pi;
    }

    int get_k() const
    {
        return k;
    }

    int get_id() const
    {
        return id;
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
