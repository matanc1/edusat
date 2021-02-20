//
// Created by matan on 23/01/2021.
//

#include <cassert>
#include <src/utils/utils.h>
#include <iostream>
#include <set>
#include "var.h"
#include "src/solver/solver.h"

Var MiniSAT::choose() {
    for (auto &[score, vars] : score2vars) {
        for (auto &v : vars) {
            if (state->var_state[v] == VarState::V_UNASSIGNED) {
                return v;
            }
        }
    }
    int sum = 0;
    std::set<Var> s;
    for (auto &[score, vars] : score2vars) {
        s.insert(vars.begin(), vars.end());
        std::cout << score << ":" << vars.size() << std::endl;
        sum += vars.size();
    }
    std::cout << *(s.rbegin()) << " " << *(s.begin()) << std::endl;
    throw std::runtime_error("ERROR - should've found an unassigned variable");
}

void MiniSAT::before_initialize_clauses(std::vector<std::vector<Lit>> &clauses) {
    activity.resize(state->nvars + 1);
}

void MiniSAT::before_add_clause(std::vector<Lit> &lits) {
    for (auto &lit : lits) {
        Var v = l2v(lit);
        update_var_score(v);
    }
}

void MiniSAT::update_var_score(Var &v) {
    double score = activity[v];

    if (score > 0) {
        assert(score2vars.find(score) != score2vars.end());
        score2vars[score].erase(v);
        if (score2vars[score].empty()) score2vars.erase(score);
    }
    activity[v] = score + var_inc;;

    // Rescaling, to avoid overflows;
    if (activity[v] > RESCALE_THRESHOLD) rescale_scores();

    if (score2vars.find(activity[v]) != score2vars.end())
        score2vars[activity[v]].insert(v);
    else
        score2vars[activity[v]] = std::unordered_set<int>({v});
}

void MiniSAT::rescale_scores() {
    std::cout << "Rescale" << std::endl;

    for (unsigned int i = 1; i <= state->nvars; i++)
        activity[i] /= RESCALE_FACTOR;

    var_inc /= RESCALE_FACTOR;
    // rebuilding the scaled-down m_Score2Vars.
    std::map<double, std::unordered_set<Var>, std::greater<double>> tmp_map;
    double prev_score = 0.0f;
    for (auto &m : score2vars) {
        double scaled_score = m.first / RESCALE_FACTOR;
        if (scaled_score == prev_score) // This can happen due to rounding
            tmp_map[scaled_score].insert(score2vars[m.first].begin(), score2vars[m.first].end());
        else
            tmp_map[scaled_score] = score2vars[m.first];
        prev_score = scaled_score;
    }
    tmp_map.swap(score2vars);
}

void MiniSAT::after_learn_clause(std::vector<Lit> &lits) {
    var_inc *= 1 / SPEEDUP_FACTOR;
}
