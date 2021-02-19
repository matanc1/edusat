//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_RUNSTATISTICSCALLBACK_H
#define EDUSAT_RUNSTATISTICSCALLBACK_H


#include "CallbackBase.h"

class RunStatisticsCallback : public CallbackBase {
public:
    int num_assignments = 0;
    int num_backtracks = 0;
    int num_learned = 0;
    bool verbose = true;

    void after_assignment(Lit &l) override {
        num_assignments++;
    }

    void after_initialize_clauses() override {
        std::cout << "There were **" << num_assignments
                  << "** assignments in initialization phase" << std::endl << std::flush;
        print_spaces();
    }

    void before_backtrack() override {
        num_backtracks++;
        if (verbose) std::cout << "Num backtracks: " << num_backtracks;
    }

    void after_learn_clause(std::vector<Lit> &lits) override {
        num_learned++;
        if (verbose) std::cout << "Num learned: " << num_learned;
    }
};

#endif //EDUSAT_RUNSTATISTICSCALLBACK_H
