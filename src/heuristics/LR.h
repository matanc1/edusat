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
    int num_learned;
    std::map<Var, int> num_learned_on_assignment; //the number of learned clauses when this variable was assigned
    std::map<Var, int> num_participated;
    std::map<Var, int> num_reasoned;
    std::map<double, std::set<Var>, std::greater<double>> lr2var;
    std::map<Var, float> var2lr;
    std::set<Var> conflict_side_vars;
    float alpha;

public:
    explicit LR(float alpha) : alpha{alpha} {}


    //PickBranchLit
    Var choose() override {
        Var v = *(lr2var.begin()->second.begin());
        assert(state->var_state[v] == VarState::V_UNASSIGNED);
        return v;
    }

    //Initialize
    void after_initialize_state() override {
        num_learned = 0;
        for (Var v = 1; v <= state->nvars; v++) {
            num_learned_on_assignment[v] = 0;
            num_participated[v] = 0;
            num_reasoned[v] = 0;
            lr2var[0].insert(v);
            var2lr[v] = 0;
        }
    }

    //AfterConflictAnalysis
    void after_learn_clause(Literals &lits) override {
        num_learned++;
        std::set<Var> learned_clause_vars;
        for (auto &l : lits) learned_clause_vars.insert(l2v(l));
        learned_clause_vars.insert(conflict_side_vars.begin(), conflict_side_vars.end());
        for (auto &v : learned_clause_vars) num_participated[v]++;
        conflict_side_vars.clear(); //reset conflict vars after learning a new clause
        if (alpha > 0.06) alpha = alpha - 1e-6;

        // RSR update
        for (auto &v : conflict_side_vars) num_reasoned[v]++;

        //locality update
        if (!lr2var.size()) {
            for (auto [lr, vars] : lr2var) {
                lr2var[lr * 0.95] = std::move(vars);
                lr2var.erase(lr);
            }
        }
    }

    void on_resolve(Literals &literals) override {
        for (auto &l : literals) conflict_side_vars.insert(l2v(l));
    }

    //onAssign
    void after_assignment(Lit &l) override {
        num_learned_on_assignment[l2v(l)] = num_learned;
        num_participated[l2v(l)] = 0;
        num_reasoned[l2v(l)] = 0;
        Var v = l2v(l);
        remove_lr(v);
    }

    //onUnassign
    void after_unassign_var(Var &v) override {
        int interval = num_learned - num_learned_on_assignment[v];
        if (interval > 0) {
            float lr = var2lr[v];
            auto r = num_participated[v] / interval;
            auto rsr = num_reasoned[v] / interval;
            auto new_lr = (1 - alpha) * lr + alpha * (r + rsr);
            insert_lr(v, new_lr);
        }
    }

    void remove_lr(Var v){
        float lr = var2lr[v];
        lr2var[lr].erase(v);
        if (lr2var[lr].empty()) lr2var.erase(lr);
    }

    void insert_lr(Var v, float new_lr) {
        lr2var[new_lr].insert(v);
        var2lr[v] = new_lr;
    }

    void add_output(std::map<std::string, std::string> &output) {
        output["vdh"] = "LR";
    }
};


#endif //EDUSAT_LR_H
