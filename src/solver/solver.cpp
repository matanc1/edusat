//
// Created by matan on 18/01/2021.
//

#include "src/parsing/dimacs_parser.h"
#include <fstream>
#include <src/utils/utils.h>
#include <memory>
#include "solver.h"
#include "exceptions.h"


Solver::Solver(std::string filename, std::shared_ptr<VarDecisionHeuristic> var,
               std::shared_ptr<ValDecisionHeuristic> val,
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
    output["status"] = "SAT";
    output["filename"] = filename;
    try {
        read_cnf();
        _solve();
        std::cout << "SAT" << std::endl;
        state->validate_assignment();
        state->write_assignment("output.txt");
    }
    catch(SatSolverException &e) {
        e.add_output(output);
        std::cout << e.what() << std::endl;
    }
    state->add_output(output);
    write_output("statistics.txt", output);
}

void Solver::_solve() {
    SolverStatus res;
    while (true) {
        while (true) {
            res = state->BCP();
            if (res == SolverStatus::CONFLICT) {
                auto dl_to_backtrack = state->analyze();
                state->backtrack(dl_to_backtrack);
            } else break;
        }
        res = decide();
        if (res == SolverStatus::SAT) return;
    }
}


SolverStatus Solver::decide() {
    if (state->all_assigned()) return SolverStatus::SAT;
    Var v = var->choose();
    Lit l = val->choose(v);
    state->decide(l);
    return SolverStatus::UNDEF;
}

void Solver::write_output(std::string file_name, std::map<std::string, std::string> &outputs) {
    std::ofstream out;
    out.open(file_name);
    for (auto &[key, value] : outputs) {
        out << key << ":" << value << std::endl;
    }
    out.close();
}


