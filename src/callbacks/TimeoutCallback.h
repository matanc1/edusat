//
// Created by Matan on 14/02/2021.
//

#ifndef EDUSAT_TIMEOUTCALLBACK_H
#define EDUSAT_TIMEOUTCALLBACK_H


#include <cassert>
#include "src/utils/utils.h"
#include <sstream>
#include <src/solver/SatSolverException.h>
#include "CallbackBase.h"

class TimeoutCallback : public CallbackBase {
    int seconds;
    double start_time;

public:
    explicit TimeoutCallback(int seconds) : seconds(seconds), start_time(cpuTime()) {
        assert(seconds > 0);
    }

    void before_bcp() override {
        std::ostringstream oss;
        oss << "Timeout of **" << seconds << "** seconds exceeded";

        if (cpuTime() - start_time > seconds) {
            throw SatSolverException(oss.str());
        }
    }

    void add_output(std::map<std::string, std::string> &output) override {
        output["timeout"] = std::to_string(seconds);
        output["runtime"] = std::to_string(cpuTime() - start_time);
    }
};


#endif //EDUSAT_TIMEOUTCALLBACK_H
