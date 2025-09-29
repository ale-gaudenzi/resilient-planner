#ifndef RESILIENT_NODE_FORMULA_H
#define RESILIENT_NODE_FORMULA_H

#include <tr1/functional>
#include "operator.h"
#include <set>

class ResilientNodeFormula {
    PartialState formula;
    int k;
    std::set<Operator> pi;
    long int id;
    Operator next_op;

public:
    ResilientNodeFormula() = default;
    ResilientNodeFormula(PartialState formula, int k_, std::set<Operator> pi_, Operator next_op_);
    ResilientNodeFormula(PartialState formula, int k_, std::set<Operator> pi_, Operator next_op_, long int id_);
    ResilientNodeFormula(PartialState formula, int k_);

    void dump() const;
    PartialState get_formula() const { return formula; }
    Operator get_next_operator() const { return next_op; }
    std::set<Operator> get_pi() const { return pi; }
    int get_k() const { return k; }
    int get_id() const { return id; }

    bool operator==(const ResilientNodeFormula &other) const { return id == other.id; }
    bool operator<(const ResilientNodeFormula &other) const { return id < other.id; }
};

namespace std {
namespace tr1 {
    template <>
    struct hash<ResilientNodeFormula> {
        size_t operator()(const ResilientNodeFormula &node_formula) const {
            return hash<int>()(node_formula.get_id());
        }
    };
}
}

#endif
