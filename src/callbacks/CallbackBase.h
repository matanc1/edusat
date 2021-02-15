//
// Created by Matan on 14/02/2021.
//

#ifndef EDUSAT_CALLBACK_H
#define EDUSAT_CALLBACK_H


#include <memory>
#include <vector>
#include <exception>

class SolverState;

class CallbackBase {
protected:
    std::shared_ptr<SolverState> state;

public:
    CallbackBase() {}

    //SOLVER CALLBACKS
    virtual void before_bcp() {}


    //STATE CALLBACKS
    virtual void after_initialization() {}

    virtual void after_assignment() {}

    virtual void before_add_clause(std::vector<int> clause) {}

    void set_state(std::shared_ptr<SolverState> state) {
        this->state = state;
    }
};

typedef std::vector<std::shared_ptr<CallbackBase>> Callbacks;

class CancelException : public std::exception {
private:
    std::string message;

public:
    CancelException(std::string message) : message(message) {};

    virtual const char* what() const throw () {
        return message.c_str();
    }

};

#endif //EDUSAT_CALLBACK_H

