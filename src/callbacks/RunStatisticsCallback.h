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
    bool verbose = false;

    void after_assignment(Lit &l) override {
        num_assignments++;
    }

    void before_backtrack() override {
        num_backtracks++;
        if (verbose) std::cout << "Num backtracks: " << num_backtracks << std::endl;
    }

    void after_learn_clause(std::vector<Lit> &lits) override {
        num_learned++;
        if (verbose) std::cout << "Num learned: " << num_learned << std::endl;
    }
};

#endif //EDUSAT_RUNSTATISTICSCALLBACK_H
