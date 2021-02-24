//
// Created by Matan on 23/02/2021.
//

#ifndef EDUSAT_LR_H
#define EDUSAT_LR_H


#include <src/utils/utils.h>
#include "var.h"
#include <map>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <set>

class LR : public VarDecisionHeuristic {
private:
    int num_learned = 0;
    std::map<Var, int> num_learned_on_assignment; //the number of learned clauses when this variable was assigned
    std::map<Var, int> num_participated;
    std::map<Var, int> num_reasoned;
    std::map<double, std::set<Var>, std::greater<double>> lr2var;
    std::map<Var, float> var2lr;
    std::set<Var> conflict_side_vars;
    float alpha;

public:
    explicit LR(float alpha) : alpha{alpha} {}


    Var choose() override {
        for (auto &[score, vars] : lr2var) {
            for (auto &v : vars) {
                if (state->var_state[v] == VarState::V_UNASSIGNED) {
//                    assert(score <= 1);
                    return v;
                }
            }
        }
    }

    void after_initialize_state() override {
        for (int v = 1; v <= state->nvars; v++) {
            num_learned_on_assignment[v] = 0;
            num_participated[v] = 0;
            lr2var[0].insert(v);
        }
    }

    void after_learn_clause(Literals &lits) override {
        num_learned++;
        std::set<Var> learned_clause_vars;
        for (auto &l : lits) learned_clause_vars.insert(l2v(l));
        learned_clause_vars.insert(conflict_side_vars.begin(), conflict_side_vars.end());
        for (auto &v : conflict_side_vars) num_reasoned[v]++;
        for (auto &v : learned_clause_vars) num_participated[v]++;
        if (alpha > 0.06) alpha = alpha - 1e-6;

        //locality
    }

    void on_resolve(Literals &literals) override {
        for (auto &l : literals) conflict_side_vars.insert(l2v(l));
    }

    void after_assignment(Lit &l) override {
        num_learned_on_assignment[l2v(l)] = num_learned;
        num_participated[l2v(l)] = 0;
        num_reasoned[l2v(l)] = 0;
    }

    void after_unassign_var(Var &v) override {
        int interval = num_learned - num_learned_on_assignment[v];
        if (interval > 0) {
            float lr = var2lr[v];
            auto r = num_participated[v] / interval;
            auto rsr = num_reasoned[v] / interval;
            auto new_lr = (1 - alpha) * lr + alpha * (r + rsr);
//            assert(lr <= 1);
            update_lr(v, new_lr);
        }
    }

    void update_lr(Var v, float new_lr) {
        float lr = var2lr[v];
        lr2var[lr].erase(v);
        if (lr2var[new_lr].empty()) lr2var.erase(new_lr);
        lr2var[new_lr].insert(v);
        var2lr[v] = new_lr;
    }

    void add_output(std::map<std::string, std::string> &output) {
        output["vdh"] = "LR";
    }
};


#endif //EDUSAT_LR_H
