//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_SOLVER_STATE_H
#define EDUSAT_SOLVER_STATE_H


#include "clause.h"

class SolverState {

public:
    std::vector<Clause> cnf;
    std::vector<std::vector<int>> watches;  // Lit => vector of clause indices into CNF
    std::vector<VarState> state;  // current assignment
    unsigned int nclauses;
    unsigned int nvars;

    void add_clause(std::vector<int> clause, int lw, int rw);

    void initialize(unsigned int nvars, unsigned int nclauses);
};


#endif //EDUSAT_SOLVER_STATE_H
