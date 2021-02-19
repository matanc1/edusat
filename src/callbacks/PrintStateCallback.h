//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_PRINTSTATECALLBACK_H
#define EDUSAT_PRINTSTATECALLBACK_H


#include "CallbackBase.h"
#include <iostream>

class PrintStateCallback : public CallbackBase {
    bool print_assignment;

public:
    explicit PrintStateCallback(bool print_assignment = false) : print_assignment(print_assignment) {}

    void before_initialize_clauses() override {
        std::cout << "PROBLEM DEFINITION" << std::endl << "____________________" << std::endl;
        std::cout << "nvars: " << state->nvars << " nclauses: " << state->nclauses;
        print_spaces();
    }

    void after_initialize_clauses() override {
        std::cout << "Clauses:" << std::endl;
        for (const auto &c : state->cnf) {
            for (const auto &l : c.literals) {
                std::cout << l2rl(l) << ":" << l << " ";
            }
            std::cout << std::endl;
        }
        std::cout.flush();

        std::cout << "Unaries:" << std::endl;
        for (const auto &l : state->unaries) {
            std::cout << l2rl(l) << ":" << l << std::endl;
        }
        print_spaces();
    }

    void before_decide(Lit &l) override {
        std::cout << "DECIDED:  " << l2rl(l) << ":" << l << std::endl;
    }


    void after_assignment(Lit &l) override {
        std::cout << "ASSIGNMENT" << std::endl << "____________________" << std::endl;
        std::cout << "Assigning:   " << l2rl(l) << ":" << l << std::endl;

        std::cout << "DL:" << state->dl << "    ";
        for (const auto &c : state->trail) {
            std::cout << c << "->";
        }
        std::cout << "END";
        std::cout << std::endl << std::flush;

        print_spaces();
    }

    void before_learn_clause(std::vector<Lit> &lits) override {
        std::cout << "(";
        for (auto &lit : lits) {
            auto l = l2rl(lit);
            std::cout << l << " ";
        }
        std::cout << ")" << std::flush;
    }

    void after_learn_clause(std::vector<Lit> &lits) override {
        std::cout << "Learned clause #" << state->cnf.size() + state->unaries.size() << ". ";
        std::cout << std::endl;
    }
};


#endif //EDUSAT_PRINTSTATECALLBACK_H
