//
// Created by matan on 18/01/2021.
//

#include "src/parsing/dimacs_parser.h"
#include <iostream>
#include <src/utils/utils.h>
#include <memory>
#include "solver.h"


Solver::Solver(std::string filename, std::shared_ptr<VarDecisionHeuristic> var, std::shared_ptr<ValDecisionHeuristic> val,
               Callbacks cbs) :
        filename(filename), var{var}, val{val}, state{std::make_shared<SolverState>(cbs)} {
    state->add_cb(var);
    var->set_state(state);
    state->add_cb(val);
    val->set_state(state);
    for (auto &c : cbs) c->set_state(state);
}


void Solver::read_cnf() {
    std::ifstream input(filename);
    dimacs_parser::skip_headers(input);
    unsigned int nvars, nclauses;
    std::tie(nvars, nclauses) = dimacs_parser::read_problem_definition(input);
    std::vector<std::vector<int>> clauses = dimacs_parser::read_clauses(input); //This calls a move c'tor!
    for (auto &clause : clauses) for (auto &lit : clause) lit = v2l(lit); //Transform to our internal representation
    state->initialize(nvars, nclauses, std::move(clauses));
}


void Solver::solve() {
    std::map<std::string, std::string> output;
    output["filename"] = filename;
    auto result = _solve();
    output["status"] = result == SolverStatus::SAT ? "SAT" : "UNSAT";
    if (result == SolverStatus::SAT) {
        state->validate_assignment();
        state->write_assignment("output.txt");
    }
    state->add_output(output);
    write_output(output);
}

SolverStatus Solver::_solve() {
    SolverStatus res;
    while (true) {
        while (true) {
            res = state->BCP();
            if (res == SolverStatus::UNSAT) return res;
            if (res == SolverStatus::CONFLICT) {
                auto dl_to_backtrack = state->analyze();
                state->backtrack(dl_to_backtrack);
            } else break;
        }
        res = decide();
        if (res == SolverStatus::SAT) return res;
    }
}


SolverStatus Solver::decide() {
    if (state->all_assigned()) return SolverStatus::SAT;
    Var v = var->choose();
    Lit l = val->choose(v);
    state->decide(l);
    return SolverStatus::UNDEF;
}

void Solver::write_output(std::map<std::string, std::string> &outputs) {
    std::ofstream out;
    out.open("statistics.txt");
    for (auto &[key, value] : outputs ) {
        out << key << ":" << value << std::endl;
    }
    out.close();
}

