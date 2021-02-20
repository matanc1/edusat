//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_PRINTSTATECALLBACK_H
#define EDUSAT_PRINTSTATECALLBACK_H


#include "CallbackBase.h"
#include <iostream>

class LocalRestart : public CallbackBase {

    int num_learned = 0;
    vector<int> conflicts_at_dl; // decision level => # of conflicts under it. Used for local restarts.

public:

    void before_initialize_clauses(std::vector<std::vector<Lit>> &clauses) {
        conflicts_at_dl.push_back(0);
    }

    void after_learn_clause(std::vector<Lit> &lits) override {
        num_learned++;
    }
};


#endif //EDUSAT_PRINTSTATECALLBACK_H
