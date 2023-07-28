#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include "resilient_node.h"
#include "resilient_policy.h"
#include <tr1/unordered_map>

using namespace std;

extern void print_resilient_nodes(tr1::unordered_map<int, ResilientNode> resilient_nodes);
extern void print_non_resilient_nodes(tr1::unordered_map<int, ResilientNode> resilient_nodes);
extern void print_branches();
extern void print_statistics(int resilient_nodes_size, int non_resilient_nodes_size);
extern void print_timings();
extern void print_memory();
extern void print_policy();
extern void print_plan(bool to_file, std::list<Operator> plan);
extern void print_resilient_policy(map<ResilientNode, Operator> policy);
extern void print_resilient_policy_json(map<ResilientNode, Operator> policy);

#endif