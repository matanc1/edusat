//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_RUNSTATISTICSCALLBACK_H
#define EDUSAT_RUNSTATISTICSCALLBACK_H


#include "CallbackBase.h"

class RunStatisticsCallback : public CallbackBase {
public:
    int num_assignments = 0;

    void after_assignment() {
        num_assignments++;
    }

    void after_initialization() override {
        std::cout << "There were **" << num_assignments
                  << "** assignments after initialization" << std::endl << std::flush;
    }
};

#endif //EDUSAT_RUNSTATISTICSCALLBACK_H
