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
    std::shared_ptr<VarDecisionHeuristic> var;
    std::shared_ptr<ValDecisionHeuristic> val;
    std::shared_ptr<SolverState> state;
    std::string filename;

    SolverStatus _solve();

public:
    Solver(std::string filename, std::shared_ptr<VarDecisionHeuristic> var, std::shared_ptr<ValDecisionHeuristic> val,
           Callbacks cbs);

    void read_cnf();

    void solve();

    SolverStatus decide();

    void write_output(std::map<std::string, std::string> &map);
};

#endif //EDUSAT_SOLVER_H
