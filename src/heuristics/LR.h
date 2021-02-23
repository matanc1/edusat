//
// Created by Matan on 23/02/2021.
//

#ifndef EDUSAT_LR_H
#define EDUSAT_LR_H


#include <src/utils/utils.h>
#include "var.h"
#include <map>
#include <unordered_set>
#include <cassert>

class LR : public VarDecisionHeuristic {
private:
    int num_learned = 0;
    std::map<Var, int> num_learned_on_assignment; //the number of learned clauses when this variable was assigned
    std::map<Var, int> num_participated_in; //the number of learned clauses when this variable was assigned
    std::map<Var, double> var2lr;
    std::map<double, Var, std::greater<double>> lr2var;
    float alpha;

public:
    explicit LR(float alpha) : alpha{alpha} {}

    Var choose() override {
        return lr2var.begin()->second;
    }

    void after_learn_clause(std::vector<Lit> &lits) override {
        num_learned++;
    }


    void after_assignment(Lit &l) override {
        Var v = l2v(l);
        num_learned_on_assignment[v] = num_learned;
        assert(num_participated_in.find(v) == num_participated_in.end());
        num_participated_in[v] = 0;
    }

    void after_unassign_var(Var &v) {
        int num_learned_start = num_learned_on_assignment[v];
        int num_learned_end = num_learned;

        double lr = num_participated_in[v] / (num_learned_end - num_learned_start);
        auto score = lr * alpha + (1 - alpha) * var2lr[v];
        var2lr[v] = score;
        lr2var[score] = v;
    }
};


#endif //EDUSAT_LR_H
