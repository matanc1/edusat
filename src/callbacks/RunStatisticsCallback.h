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
    int num_decisions = 0;

    void after_assignment(Lit &l) override {
        num_assignments++;
    }

    void after_decide(Lit &l) override {
        num_decisions++;
    }

    void before_backtrack(int k) override {
        num_backtracks++;
    }

    void after_learn_clause(std::vector<Lit> &lits) override {
        num_learned++;
    }


    void add_output(std::map<std::string, std::string> &output) override {
        output["num_learned"] = std::to_string(num_learned);
        output["num_backtracks"] = std::to_string(num_backtracks);
        output["num_decisions"] = std::to_string(num_decisions);
        output["num_assignments"] = std::to_string(num_assignments);
        output["nvars"] = std::to_string(state->nvars);
        output["nclauses"] = std::to_string(state->nclauses);
    }
};

#endif //EDUSAT_RUNSTATISTICSCALLBACK_H
