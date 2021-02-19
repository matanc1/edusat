//
// Created by matan on 23/01/2021.
//

#include <src/utils/utils.h>
#include "val.h"
#include "src/solver/solver.h"


Lit LitScore::choose(Var v) {
    int litp = v2l(v), litn = v2l(-v);
    int pScore = lit_score[litp], nScore = lit_score[litn];
    return pScore > nScore ? litp : litn;
}

void LitScore::after_initialize_state() {
    auto nlits = 2 * state->nvars;
    lit_score.resize(nlits + 1);
}

void LitScore::before_add_clause(std::vector<Lit> &lits) {
    for (auto &lit : lits) lit_score[lit]++;
}

void LitScore::before_initialize_clauses() {
    lit_score.resize(state->nvars + 1);
}
