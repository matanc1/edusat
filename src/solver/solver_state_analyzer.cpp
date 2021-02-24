//
// Created by Matan on 23/02/2021.
//

#include <src/utils/utils.h>
#include "solver_state.h"
#include <algorithm>

int SolverState::analyze() {
    Clause current_clause = cnf[conflicting_clause_idx];
    Literals new_literals;

    int resolve_num = 0,
            bktrk = 0,
            watch_lit = 0; // points to what literal in the learnt clause should be watched, other than the asserting one

    Lit u;
    Var v;
    do {
        on_resolve(current_clause.literals);
        //NEW CLAUSE ALL LITERALS FROM CURRENT LEVEL
        for (auto &lit : current_clause.literals) { //iterate over all the literals
            Var v = l2v(lit);
            if (!marked[v]) { //mark all the literals that are in the clause (resolution candidates)
                marked[v] = true;
                /*
                 * If the dlevel of v is as current, increase resolve_num.
                 * This is the #of resolutions we need to do (remove all the vars
                 * from current DL except one which'll become unitary).
                 */
                if (dlevel[v] == dl) ++resolve_num;
                else { // literals from previous decision levels (roots) are entered to the learned clause.
                    new_literals.push_back(lit);
                    int c_dl = dlevel[v];
                    if (c_dl > bktrk) {
                        bktrk = c_dl;
                        watch_lit = new_literals.size() - 1; //So the watch'll be on an unassigned var afterwards
                    }
                }
            }
        }
        u = last_assigned_literal();
        v = l2v(u);
        marked[v] = false;
        resolve_num--;
        if (!resolve_num) continue;
        current_clause = next_conflict_candidates(u, antecedent[v]);
    } while (resolve_num > 0);
    for (auto &lit : new_literals)
        marked[l2v(lit)] = false;
    Lit negated_u = negate(u);
    new_literals.push_back(negated_u);
    assigned_lit = negated_u;
    add_learned_clause(new_literals, watch_lit, new_literals.size() - 1);
    return bktrk;
}

Clause SolverState::next_conflict_candidates(Lit u, int ant) {
    Clause current_clause = cnf[ant];
    current_clause.literals.erase(find(current_clause.literals.begin(), current_clause.literals.end(), u));
    return current_clause;
}

Lit SolverState::last_assigned_literal() {
    for (auto t_it = trail.rbegin(); t_it != trail.rend(); t_it++) {
        Lit lit = *t_it;
        if (marked[l2v(lit)]) return lit;
    }
    throw std::runtime_error("Should always find a literal");
}

void SolverState::on_resolve(Literals &vector) {
    for (const auto &cb : cbs) cb->on_resolve(vector);
}

