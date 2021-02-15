//
// Created by Matan on 15/02/2021.
//

#include "solver_state.h"
#include <iostream>
#include "src/utils/utils.h"

void SolverState::add_clause(std::vector<int> clause, int lw, int rw) {
    Literals literals;
    for (auto c : clause) literals.push_back(v2l(c));

    Clause c{literals};
    c.lw = lw;
    c.rw = rw;

    auto next_index = cnf.size();
    watches.at(c.literals[lw]).push_back(next_index);
    watches.at(c.literals[rw]).push_back(next_index);
    cnf.push_back(c);
}

void SolverState::initialize(unsigned int nvars, unsigned int nclauses) {
    this->nvars = nvars;
    this->nclauses = nclauses;
    state.resize(nvars + 1, VarState::V_UNASSIGNED);

    auto nlits = 2 * nvars;
    watches.resize(nlits + 1);
}
