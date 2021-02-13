//
// Created by Matan on 14/02/2021.
//

#ifndef EDUSAT_TIMEOUTCALLBACK_H
#define EDUSAT_TIMEOUTCALLBACK_H


#include <cassert>
#include "src/utils/utils.h"
#include <sstream>
#include "CallbackBase.h"

class TimeoutCallback : public CallbackBase {
    int limit;
    double start_time;

public:
    explicit TimeoutCallback(int limit) : limit(limit), start_time(cpuTime()) {
        assert(limit > 0);
    }

    void before_bcp() override;
};


#endif //EDUSAT_TIMEOUTCALLBACK_H
