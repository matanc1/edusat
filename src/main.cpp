//
// Created by matan on 16/01/2021.
//



/*
#include "heuristics/var.h"
#include "heuristics/val.h"
#include "solver/solver.h"
*/
#include <iostream>
#include <fstream>
#include <src/heuristics/var.h>
#include <src/heuristics/val.h>
#include <src/solver/solver.h>

int main(int argc, char **argv) {

    LR lr;
    LitScore lit_score;
    VarDecisionHeuristic &var = lr;
    ValDecisionHeuristic &val = lit_score;

    std::ifstream input(argv[argc - 1]);
    Solver s(var, val);
    s.read_cnf(input);
    s.solve();
    std::cout << "DONE" << std::endl;
}

