//
// Created by matan on 16/01/2021.
//

#include <iostream>
#include "src/heuristics/var.h"
#include "src/heuristics/val.h"
#include "src/solver/solver.h"
#include "src/callbacks/TimeoutCallback.h"
#include <memory>
#include <src/callbacks/RunStatisticsCallback.h>
#include "src/callbacks/PrintStateCallback.h"
#include "src/callbacks/HandleUnaryClausesCallback.h"

int main(int argc, char **argv) {
    LR lr;
    LitScore lit_score;
    VarDecisionHeuristic &var = lr;
    ValDecisionHeuristic &val = lit_score;


    std::shared_ptr<TimeoutCallback> timeout_callback = std::make_shared<TimeoutCallback>(2);
    std::shared_ptr<PrintStateCallback> print_state_callback = std::make_shared<PrintStateCallback>();
    std::shared_ptr<HandleUnaryClausesCallback> unary_clauses_callback =
            std::make_shared<HandleUnaryClausesCallback>();
    std::shared_ptr<RunStatisticsCallback> run_statistics_callback =
            std::make_shared<RunStatisticsCallback>();


    Callbacks solver_cbs = {
            timeout_callback
    };

    Callbacks state_cbs = {
            print_state_callback,
            unary_clauses_callback,
            run_statistics_callback
    };

    std::ifstream input(argv[argc - 1]);
    Solver s(var, val, solver_cbs, state_cbs);
    s.read_cnf(input);
    s.solve();
    std::cout << "DONE" << std::endl;
}

