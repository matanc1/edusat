//
// Created by matan on 16/01/2021.
//



#include <iostream>
#include <fstream>
#include "src/heuristics/var.h"
#include "src/heuristics/val.h"
#include "src/solver/solver.h"
#include "src/callbacks/TimeoutCallback.h"
#include <memory>
#include "src/callbacks/PrintStateCallback.h"

int main(int argc, char **argv) {

    LR lr;
    LitScore lit_score;
    VarDecisionHeuristic &var = lr;
    ValDecisionHeuristic &val = lit_score;


    std::shared_ptr<TimeoutCallback> timeout_callback = std::make_shared<TimeoutCallback>(2);
    std::shared_ptr<PrintStateCallback> print_state_callback = std::make_shared<PrintStateCallback>();


    Callbacks callbacks;
    callbacks.push_back(timeout_callback);
    callbacks.push_back(print_state_callback);

    std::ifstream input(argv[argc - 1]);
    Solver s(var, val, callbacks);
    s.read_cnf(input);
    s.solve();
    std::cout << "DONE" << std::endl;
}

