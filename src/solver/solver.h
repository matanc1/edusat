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


enum class SolverState {
    UNSAT,
    SAT,
    CONFLICT,
    UNDEF,
    TIMEOUT
};


class Clause {
    typedef std::vector<Lit> clause_t;

    clause_t clause;
    int lw, rw; //watches;

public:
    Clause() = default;

    Clause(clause_t c) : clause{std::move(c)} {
        lw = 0;
        rw = 1;
        //TODO: Set LW, RW
    }


    friend std::ostream &operator<<(std::ostream &out, const Clause &c);
};


class Solver {
private:
    VarDecisionHeuristic &var;
    ValDecisionHeuristic &val;
    std::vector<Clause> cnf;

    SolverState _solve(Callbacks &cbs);
    void before_bcp(Callbacks &cbs);

public:
    unsigned int nclauses;
    unsigned int nvars;


    Solver(VarDecisionHeuristic &var, ValDecisionHeuristic &val);

    void read_cnf(std::ifstream &input, bool verbose = false);

    void solve(Callbacks &cbs);
};

#endif //EDUSAT_SOLVER_H
