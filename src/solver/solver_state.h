//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_SOLVER_STATE_H
#define EDUSAT_SOLVER_STATE_H

#include <vector>
#include "src/utils/definitions.h"
#include "clause.h"
#include "src/callbacks/CallbackBase.h"

class SolverState {

    void initialize_clauses(std::vector<std::vector<Lit>> clauses);
    void initialize_state(unsigned int nvars, unsigned int nclauses);
    void before_add_clause(std::vector<int> &vector);
    void after_initialization();
    void after_assignment();

public:
    std::vector<Clause> cnf;
    std::vector<std::vector<int>> watches;  // Lit => vector of clause indices into CNF
    std::vector<VarState> var_state;  // current assignment
    std::vector<Lit> unaries;
    std::vector<int> dlevel; // var => decision level in which this variable was assigned its value.
    std::vector<Lit> trail; // assignment stack
    unsigned int nclauses;
    unsigned int nvars;

    int dl; // current decision level
    Callbacks cbs;

    SolverState(Callbacks cbs);

    void add_clause(std::vector<int> dimacs_clauses, int lw, int rw);

    void initialize(unsigned int nvars, unsigned int nclauses, std::vector<std::vector<Lit>> clauses);

    void assign_literal(Lit l);
};


#endif //EDUSAT_SOLVER_STATE_H
