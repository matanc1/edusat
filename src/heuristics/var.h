//
// Created by matan on 23/01/2021.
//

#ifndef EDUSAT_VAR_H
#define EDUSAT_VAR_H


#include "src/utils/definitions.h"
#include <map>
#include <src/callbacks/CallbackBase.h>
#include <unordered_set>

class VarDecisionHeuristic : public CallbackBase {

public:
    virtual Var choose() = 0;
};

class MiniSAT : public VarDecisionHeuristic {
    std::map<double, std::unordered_set<Var>, std::greater<double>> score2vars; //'greater' forces an order from large to small of the keys

    std::vector<double>	activity; // Var => activity
    double var_inc = 1.0;	// current increment of var score (it increases over time)
    const double RESCALE_THRESHOLD = 1e6;
    const double RESCALE_FACTOR = 1e3;
    const double SPEEDUP_FACTOR = 1.01;

    void rescale_scores();

public:
    Var choose() override;

    void before_initialize_clauses() override;

    void before_add_clause(std::vector<Lit> &lits) override;
    void after_learn_clause(std::vector<Lit> &lits) override;

    void update_var_score(Var &v);
};
#endif //EDUSAT_VAR_H
