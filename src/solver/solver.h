//
// Created by matan on 18/01/2021.
//

#ifndef EDUSAT_SOLVER_H
#define EDUSAT_SOLVER_H


#include <fstream>
#include <vector>
#include <src/callbacks/CallbackBase.h>
#include "src/heuristics/var.h"
#include "src/heuristics/val.h"
#include "solver_state.h"


class Solver {
private:
    VarDecisionHeuristic &var;
    ValDecisionHeuristic &val;
    std::shared_ptr<SolverState> state;
    Callbacks cbs;

    void _solve();
    SolverStatus BCP();

    void before_bcp();

public:
    Solver(VarDecisionHeuristic &var, ValDecisionHeuristic &val, Callbacks solver_cbs, Callbacks state_cbs);

    void read_cnf(std::ifstream &input);

    void solve();

    SolverStatus decide();

};

#endif //EDUSAT_SOLVER_H
