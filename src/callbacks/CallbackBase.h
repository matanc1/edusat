//
// Created by Matan on 14/02/2021.
//

#ifndef EDUSAT_CALLBACK_H
#define EDUSAT_CALLBACK_H


#include <memory>
#include <vector>
#include <exception>
#include "src/utils/definitions.h"
#include <iostream>
#include <string>
#include <map>

class SolverState;

class CallbackBase {
protected:
    std::shared_ptr<SolverState> state;

public:
    CallbackBase() {}

    //SOLVER CALLBACKS
    virtual void before_bcp() {}
    virtual void after_bcp() {}


    //STATE CALLBACKS
    virtual void before_initialize_state() {}
    virtual void after_initialize_state() {}

    virtual void on_resolve(Literals &literals) {}

    virtual void before_initialize_clauses(std::vector<std::vector<Lit>> &clauses) {}
    virtual void after_initialize_clauses() {}

    virtual void before_assignment(Lit &l) {}
    virtual void after_assignment(Lit &l) {}

    virtual void before_backtrack(int k) {}

    virtual void before_add_clause(std::vector<int> &clause) {}

    virtual void before_learn_clause(std::vector<Lit> &lits) {}

    virtual void after_learn_clause(std::vector<Lit> &lits) {}

    virtual void before_decide(Lit &l) {}
    virtual void after_decide(Lit &l) {}
    virtual void add_output(std::map<std::string, std::string> &output) {}

    virtual void before_unassign_var(Var &v) {}
    virtual void after_unassign_var(Var &v) {}

    void set_state(std::shared_ptr<SolverState> state) {
        this->state = state;
    }

    void print_spaces(int n=2) {
        for (int i = 0; i<n; i++) std::cout << std::endl;
    }
};

typedef std::vector<std::shared_ptr<CallbackBase>> Callbacks;

/*
class CancelException : public std::exception {
private:
    std::string message;

public:
    CancelException(std::string message) : message(message) {};

    virtual const char *what() const throw() {
        return message.c_str();
    }

};
*/

#endif //EDUSAT_CALLBACK_H

