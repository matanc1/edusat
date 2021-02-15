//
// Created by Matan on 15/02/2021.
//

#include "solver_state.h"
#include "src/utils/utils.h"


void SolverState::add_clause(std::vector<int> literals, int lw, int rw) {
    before_add_clause(literals);
    Clause c{literals};
    c.lw = lw;
    c.rw = rw;

    auto next_index = cnf.size();
    watches.at(c.literals[lw]).push_back(next_index);
    watches.at(c.literals[rw]).push_back(next_index);
    cnf.push_back(c);
}


void SolverState::initialize(unsigned int nvars, unsigned int nclauses, std::vector<std::vector<Lit>> clauses) {
    this->initialize_state(nvars, nclauses);
    this->initialize_clauses(clauses);
    after_initialization();
}

void SolverState::initialize_state(unsigned int nvars, unsigned int nclauses) {
    this->nvars = nvars;
    this->nclauses = nclauses;
    dl = 0;
    var_state.resize(nvars + 1, VarState::V_UNASSIGNED);
    dlevel.resize(nvars + 1);

    auto nlits = 2 * nvars;
    watches.resize(nlits + 1);
}

void SolverState::initialize_clauses(std::vector<std::vector<Lit>> clauses) {
    for (auto &c: clauses) {
        try {
            add_clause(c, 0, static_cast<int>(c.size()) - 1);
        }
        catch (const CancelException &e){
            continue;
        }
    }
}

SolverState::SolverState(Callbacks cbs) : cbs(cbs) {}

void SolverState::assign_literal(Lit l) {
    trail.push_back(l);
    auto var = l2v(l);
    var_state[var] = (Neg(l)) ? VarState::V_FALSE : VarState::V_TRUE;
    dlevel[var] = dl;

    after_assignment();
}

void SolverState::before_add_clause(std::vector<int> &vector) {
    for (const auto &cb : cbs) cb->before_add_clause(vector);
}

void SolverState::after_initialization() {
    for (const auto &cb : cbs) {
        cb->after_initialization();
    }
}

void SolverState::after_assignment() {
    for (const auto &cb : cbs) cb->after_assignment();
}
