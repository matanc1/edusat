//
// Created by matan on 18/01/2021.
//

#include "src/parsing/dimacs_parser.h"
#include <iostream>
#include <src/utils/utils.h>
#include <memory>



Solver::Solver(VarDecisionHeuristic &var, ValDecisionHeuristic &val, Callbacks solver_cbs, Callbacks state_cbs) :
        var{var}, val{val}, cbs(solver_cbs), state{std::make_shared<SolverState>(state_cbs)} {
    for (auto &c : solver_cbs) c->set_state(state);
    for (auto &c : state_cbs) c->set_state(state);
}


void Solver::read_cnf(std::ifstream &input) {
    dimacs_parser::skip_headers(input);
    unsigned int nvars, nclauses;
    std::tie(nvars, nclauses) = dimacs_parser::read_problem_definition(input);
    std::vector<std::vector<int>> clauses = dimacs_parser::read_clauses(input); //This calls a move c'tor!
    for (auto &clause : clauses) for (auto &lit : clause) lit = v2l(lit); //Transform to our internal representation
    state->initialize(nvars, nclauses, std::move(clauses));
}


void Solver::solve() {
    try {
        _solve();
    }
    catch (const CancelException &e) {
        std::cout << e.what();
    }
}

void Solver::_solve() {
    SolverStatus res;
    while (true) {
        while (true) {
            res = BCP();
            if (res == SolverStatus::CONFLICT) {
/*
                auto dl_to_backtrack = analyze(cnf[conflicting_clause_idx]);
*/
                state->backtrack(0);
            }
            else break;
        }
        res = decide();
//        if (res == SolverStatus::SAT) return res;
    }
}


SolverStatus Solver::decide() {
    return SolverStatus::UNDEF;
}

void Solver::before_bcp() {
    for (const auto &cb : cbs) cb->before_bcp();
}

SolverStatus Solver::BCP() {
    before_bcp();
    return state->BCP();
}

