//
// Created by matan on 23/01/2021.
//

#ifndef EDUSAT_VAL_H
#define EDUSAT_VAL_H

#include <src/callbacks/CallbackBase.h>
#include "src/utils/definitions.h"

class ValDecisionHeuristic : public CallbackBase {
public:
    virtual Lit choose(Var v) = 0;
};

class LitScore : public ValDecisionHeuristic {
private:
    std::vector<int> lit_score;

public:
    Lit choose(Var v) override;

    void before_initialize_clauses(std::vector<std::vector<Lit>> &clauses) override;
    void before_add_clause(std::vector<Lit> &lits) override;
    void after_initialize_state() override;
};

#endif //EDUSAT_VAL_H
