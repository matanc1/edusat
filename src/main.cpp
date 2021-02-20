//
// Created by matan on 16/01/2021.
//

#include <iostream>
#include "src/solver/solver.h"
#include "src/callbacks/TimeoutCallback.h"
#include <memory>
#include <src/callbacks/RunStatisticsCallback.h>
#include "src/callbacks/PrintStateCallback.h"

int main(int argc, char **argv) {
    std::shared_ptr<VarDecisionHeuristic> var = std::make_shared<MiniSAT>();
    std::shared_ptr<ValDecisionHeuristic> val = std::make_shared<LitScore>();


    std::shared_ptr<TimeoutCallback> timeout_callback = std::make_shared<TimeoutCallback>(10);
    std::shared_ptr<PrintStateCallback> print_state_callback = std::make_shared<PrintStateCallback>();
    std::shared_ptr<RunStatisticsCallback> run_statistics_callback =
            std::make_shared<RunStatisticsCallback>();

    Callbacks cbs = {
            timeout_callback,
//            print_state_callback,
            run_statistics_callback,
    };

    Solver s(argv[argc - 1], var, val, cbs);
    s.read_cnf();
    s.solve();
}

