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

    void before_initialize_clauses(std::vector<std::vector<Lit>> &clauses) override {
        std::cout << "PROBLEM DEFINITION" << std::endl << "____________________" << std::endl;
        std::cout << "nvars: " << state->nvars << " nclauses: " << state->nclauses << std::endl;
        std::cout << "Clauses:" << std::endl;
        for (const auto &c : clauses) {
            for (const auto &l : c) {
                std::cout << l2rl(l) << ":" << l << " ";
            }
            std::cout << std::endl;
        }
        std::cout.flush();
        print_spaces();
    }

    void before_decide(Lit &l) override {
        std::cout << "DECIDED:  " << l2rl(l) << ":" << l << std::endl;
    }

    void before_backtrack() override {
        std::cout << "BEFORE BACKTRACK" << std::endl;
    }

    void after_assignment(Lit &l) override {
        std::cout << "ASSIGNING: " << l2rl(l) << ":" << l << "  ----  ";
        std::cout << "DL:" << state->dl << "    ";
        print_lits(state->trail);
        print_vars(state->trail);
        print_spaces(1);
    }

    void print_lits(std::vector<Lit> &lits, std::string seperator="->") const {
        for (const auto &l : lits) {
            std::cout << l << seperator;
        }
        std::cout << "END";
        std::cout << "    ";
    }

    void print_vars(std::vector<Lit> &lits, std::string seperator = "->") const {
        for (const auto &l : lits) {
            std::cout << "(" << l2rl(l) << ")" << seperator;
        }
        std::cout << "END";
        std::cout << "    ";
    }

    void before_learn_clause(std::vector<Lit> &lits) override {
        std::cout << "Learned clause: ";
        print_vars(lits);
        print_spaces();
    }

    void before_bcp(){
        print_spaces(1);
        std::cout << "STARTING BCP" << std::endl;
    }

    void after_bcp(){
        std::cout << "FINISHED BCP" << std::endl;
        print_spaces(1);
        if (state->conflicting_clause_idx >= 0){
            std::cout << "CONFLICT FOUND: ";
            print_lits(state->cnf[state->conflicting_clause_idx].literals, ",");
            print_vars(state->cnf[state->conflicting_clause_idx].literals, ",");
        }
        print_spaces();
    }
};


#endif //EDUSAT_PRINTSTATECALLBACK_H
