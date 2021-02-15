//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_PRINTSTATECALLBACK_H
#define EDUSAT_PRINTSTATECALLBACK_H


#include "CallbackBase.h"
#include <iostream>

class PrintStateCallback : public CallbackBase {

public:
    explicit PrintStateCallback() {}

    void after_add_clauses() override {
        std::cout << "nvars: " << state->nvars << " nclauses: " << state->nclauses << std::endl;
        std::cout << "Clauses:" << std::endl;
        for (const auto &c : state->cnf) {
            for(const auto &l : c.literals) {
                std::cout << l2rl(l) << ":" << l << " ";
            }
            std::cout << std::endl;
        }
    }
};


#endif //EDUSAT_PRINTSTATECALLBACK_H
