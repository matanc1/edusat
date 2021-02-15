//
// Created by matan on 18/01/2021.
//

#include "src/parsing/dimacs_parser.h"
#include "solver.h"
#include "../callbacks/CallbackBase.h"
#include <set>
#include <iostream>
#include <src/callbacks/CallbackBase.h>
#include <src/utils/utils.h>


Solver::Solver(VarDecisionHeuristic &var, ValDecisionHeuristic &val, Callbacks cbs) : var{var}, val{val},
                    cbs(cbs), state(std::make_shared<SolverState>()) {
    int x = 3;
    for (auto &c : cbs) c->set_state(state);
}


void Solver::read_cnf(std::ifstream &input) {
    dimacs_parser::skip_headers(input);
    unsigned int nvars, nclauses;
    std::tie(nvars, nclauses) = dimacs_parser::read_problem_definition(input);
    std::vector<std::vector<int>> clauses = dimacs_parser::read_clauses(input); //This calls a move c'tor!
    state->initialize(nvars, nclauses);
    add_all_clauses(std::move(clauses));
}


void Solver::solve() {
    SolverStatus res = _solve();
    switch (res) {
        case SolverStatus::SAT : {
            std::cout << "SAT" << std::endl;
            break;
        }
        case SolverStatus::UNSAT : {
            std::cout << "UNSAT" << std::endl;
            break;
        }
    }
}

SolverStatus Solver::_solve() {
    SolverStatus res;
    while (true) {
        before_bcp();
    }
    return SolverStatus::SAT;
}

void Solver::before_bcp() {
    for (const auto &cb : cbs) cb->before_bcp();
}

void Solver::add_all_clauses(std::vector<std::vector<int>> clauses) {
    for (auto &c: clauses) {
        state->add_clause(c, 0, static_cast<int>(c.size()) - 1);
    }
    after_add_clauses();

/*
        if (c.size() == 1) {
            Lit l = c.literals[0];
            if (state[l2v(l)] == VarState::V_UNASSIGNED) add_unary_clause(c);
            else {
                if (Neg(l) != (state[l2v(l)] == VarState::V_FALSE)){
                    throw std::runtime_error("UNSAT - Conflicting Unaries for var: " + l2v(l))
                }
            }
        } else {
        }
*/

}

void Solver::after_add_clauses() {
    for (const auto &cb : cbs) cb->after_add_clauses();
}

