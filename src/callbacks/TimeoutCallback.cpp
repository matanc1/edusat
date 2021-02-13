//
// Created by Matan on 14/02/2021.
//

#include "TimeoutCallback.h"


void TimeoutCallback::before_bcp() {
    std::ostringstream oss;
    oss << "Timeout of **" << limit << "** seconds exceeded";

    if(cpuTime() - start_time > limit) {
        throw std::runtime_error(oss.str());
    }
}
