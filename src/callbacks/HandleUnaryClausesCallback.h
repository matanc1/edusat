//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_HANDLEUNARYCLAUSESCALLBACK_H
#define EDUSAT_HANDLEUNARYCLAUSESCALLBACK_H

#include <src/utils/definitions.h>
#include "CallbackBase.h"

class HandleUnaryClausesCallback : public CallbackBase {

public:
    explicit HandleUnaryClausesCallback() {}


    void before_add_clause(std::vector<int> clause) override {
        if (clause.size() > 1) return;
        add_unary_clause_at_initialization(clause);
        throw CancelException("Added unary clause. Cancel adding to regular clauses");
    }

    void add_unary_clause_at_initialization(std::vector<int> &clause) {
        Lit l = clause.at(0);
        if (state->var_state.at(l2v(l)) == VarState::V_UNASSIGNED) {
            state->unaries.push_back(l);
            state->assign_literal(l);
        } else if (Neg(l) != (state->var_state[l2v(l)] == VarState::V_FALSE))
            throw std::runtime_error("UNSAT - Conflicting unaries for " + l2v(l));
    }

    void after_initialization() override {
        std::cout << "There were **" << state->unaries.size()
                  << "** unaries in the CNF" << std::endl << std::flush;
    }
};

#endif //EDUSAT_HANDLEUNARYCLAUSESCALLBACK_H
