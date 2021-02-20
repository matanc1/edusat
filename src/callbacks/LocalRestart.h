//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_LOCALRESTART_H
#define EDUSAT_LOCALRESTART_H


#include "CallbackBase.h"
#include <iostream>
#include <vector>

class LocalRestart : public CallbackBase {

    int num_learned = 0;
    std::vector<int> conflicts_at_dl; // decision level => # of conflicts under it. Used for local restarts.
    int threshold = 100;
    float restart_multiplier = 1.1f;
    int upper_limit = 1000;
    int lower_limit = 1000;
    int num_restarts = 0;

public:

    void before_initialize_clauses(std::vector<std::vector<Lit>> &clauses) {
        conflicts_at_dl.push_back(0);
    }

    void after_learn_clause(std::vector<Lit> &lits) override {
        num_learned++;
    }

    void before_backtrack(int k) override {
        if (k > 0 && (num_learned - conflicts_at_dl[k] > threshold)){
            restart();
            throw SkipException("Skipping backtrack after restart");
        }
    }

    void add_output(std::map<std::string, std::string> &output) override {
        output["num_restarts"] = std::to_string(num_restarts);
    }

    void restart() {
        threshold = static_cast<int>(threshold * restart_multiplier);
        if (threshold > upper_limit) {
            threshold = lower_limit;
            upper_limit = static_cast<int>(upper_limit  * restart_multiplier);
        }
        num_restarts++;
        for (unsigned int i = 1; i <= state->nvars; ++i)
            if (state->dlevel[i] > 0) {
                state->unassign_var(i);
            }
        state->trail.clear();
        state->qhead = 0;
        state->separators.clear();
        conflicts_at_dl.clear();
        conflicts_at_dl.push_back(0);
        state->reset();
    }
};


#endif //EDUSAT_LOCAL_RESTART_H
