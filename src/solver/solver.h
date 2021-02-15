//
// Created by matan on 18/01/2021.
//

#ifndef EDUSAT_SOLVER_H
#define EDUSAT_SOLVER_H


#include <fstream>
#include <utility>
#include <vector>
#include <src/callbacks/CallbackBase.h>
#include "src/heuristics/var.h"
#include "src/heuristics/val.h"
#include "clause.h"
#include "solver_state.h"
#include <exception>


class Solver {
private:
    VarDecisionHeuristic &var;
    ValDecisionHeuristic &val;
    std::shared_ptr<SolverState> state;
    Callbacks cbs;

    SolverStatus _solve();

    void before_bcp();

public:
    Solver(VarDecisionHeuristic &var, ValDecisionHeuristic &val, Callbacks cbs);

    void read_cnf(std::ifstream &input);

    void solve();

    void add_all_clauses(std::vector<std::vector<int>> clauses);

    void add_clause(std::vector<int> &clause, int lw, int rw);

    void after_add_clauses();
};

#endif //EDUSAT_SOLVER_H
