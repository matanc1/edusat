//
// Created by matan on 18/01/2021.
//

#include "src/parsing/dimacs_parser.h"
#include "solver.h"
#include <set>
#include <iostream>
#include <src/callbacks/CallbackBase.h>


Solver::Solver(VarDecisionHeuristic &var, ValDecisionHeuristic &val) : var{var}, val{val} {}


void Solver::read_cnf(std::ifstream &input, bool verbose) {
    dimacs_parser::skip_headers(input);
    std::tie(nvars, nclauses) = dimacs_parser::read_problem_definition(input);
    cnf = dimacs_parser::read_clauses(input); //This calls a move c'tor!


    if (verbose) {
        std::cout << "nvars: " << nvars << " nclauses: " << nclauses << std::endl;
        std::cout << "Clauses:" << std::endl;
        for (auto c : cnf) std::cout << c << std::endl;
    }
}


void Solver::solve(Callbacks &cbs) {
    SolverState res = _solve(cbs);
    switch (res) {
        case SolverState::SAT : {
            std::cout << "SAT" << std::endl;
            break;
        }
        case SolverState::UNSAT : {
            std::cout << "UNSAT" << std::endl;
            break;
        }
    }
}

SolverState Solver::_solve(Callbacks &cbs) {
    SolverState res;
    while (true) {
        before_bcp(cbs);
        res = BCP();
        if (res == SolverState::CONFLICT) {
            backtrack(analyze(cnf[conflict_clause_idx]));
        } else break;
    }
    res = decide();
    if (res == SolverState::SAT) return res;
}

void Solver::before_bcp(Callbacks &cbs) {
    for (const auto &cb : cbs) cb->before_bcp();
}

/*
SolverState Solver::_solve() {
    SolverState res;
    while (true) {
        before_bcp(); //TODO: Here we'll check the time and throw an exception if we timeout
        while (true) {
            res = BCP();
            if (res == SolverState::UNSAT) return res;
            if (res == SolverState::CONFLICT) {
                int dl = analyze(cnf[conflicting_clause_idx]);
                after_analyze();
                backtrack(dl);
                after_backtrack();
            } else break;

            res = decide();
            after_decide();
            if (res == SolverState::SAT) return res;
        }

    }
}


int Solver::analyze(const Clause conflicting) {

}

*/
std::ostream &operator<<(std::ostream &out, const Clause &clause) {
    for (auto &literal: clause.clause) {
        out << literal << ' ';
    }
    return out;
}
