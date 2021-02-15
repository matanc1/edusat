//
// Created by Matan on 14/02/2021.
//

#ifndef EDUSAT_CALLBACK_H
#define EDUSAT_CALLBACK_H


#include <memory>
#include <vector>
#include <src/solver/solver_state.h>

class CallbackBase {
protected:
    std::shared_ptr<SolverState> state;

public:
    CallbackBase() {}

    virtual void before_bcp() {}

    virtual void after_add_clauses() {}

    void set_state(std::shared_ptr<SolverState> state) {
        this->state = state;
    }
};

typedef std::vector<std::shared_ptr<CallbackBase>> Callbacks;

#endif //EDUSAT_CALLBACK_H
