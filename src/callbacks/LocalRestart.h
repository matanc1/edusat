//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_PRINTSTATECALLBACK_H
#define EDUSAT_PRINTSTATECALLBACK_H


#include "CallbackBase.h"
#include <iostream>

class LocalRestart : public CallbackBase {
/*
 * local restart means that we restart if the number of conflicts learned in this
 * decision level has passed the threshold.
 */

public:
    explicit LocalRestart() {}

    void before_backtrack() override {
        if (k > 0 && (num_learned - conflicts_at_dl[k] > restart_threshold)) {    // "local restart"
            restart();
            return;
        }
    }

    void after_initialization() override {
    }
};


#endif //EDUSAT_PRINTSTATECALLBACK_H
