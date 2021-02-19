//
// Created by Matan on 15/02/2021.
//

#include "solver_state.h"
#include "src/utils/utils.h"
#include <string>
#include <fstream>
#include <cassert>
#include <algorithm>


void SolverState::add_clause(std::vector<int> literals, int lw, int rw) {
    before_add_clause(literals);
    if (literals.size() == 1) _add_unary_clause(literals);
    else _add_clause(literals, lw, rw);
}

void SolverState::_add_clause(const std::vector<int> &literals, int lw, int rw) {
    Clause c{literals};
    c.lw = lw;
    c.rw = rw;

    auto next_index = cnf.size();
    watches.at(c.literals[lw]).push_back(next_index);
    watches.at(c.literals[rw]).push_back(next_index);
    cnf.push_back(c);
}


void SolverState::initialize(unsigned int nvars, unsigned int nclauses, std::vector<std::vector<Lit>> clauses) {
    before_initialize_state();
    this->initialize_state(nvars, nclauses);
    after_initialize_state();
    before_initialize_clauses(clauses);
    this->initialize_clauses(clauses);
    after_initialize_clauses();
}

void SolverState::initialize_state(unsigned int nvars, unsigned int nclauses) {
    this->nvars = nvars;
    this->nclauses = nclauses;
    dl = 0;
    max_dl = 0;
    conflicting_clause_idx = -1;
    var_state.resize(nvars + 1, VarState::V_UNASSIGNED);
    dlevel.resize(nvars + 1);
    antecedent.resize(nvars + 1, -1);
    marked.resize(nvars + 1);

    auto nlits = 2 * nvars;
    watches.resize(nlits + 1);
    separators.push_back(0);
}

void SolverState::initialize_clauses(std::vector<std::vector<Lit>> clauses) {
    for (auto &c: clauses) {
        add_clause(c, 0, static_cast<int>(c.size()) - 1);
    }
}

SolverState::SolverState(Callbacks cbs) : cbs(cbs) {}

void SolverState::assign_literal(Lit l) {
    before_assignment(l);
    _assign_literal(l);
    after_assignment(l);
}

void SolverState::_assign_literal(Lit l) {
    trail.push_back(l);
    auto var = l2v(l);
    var_state[var] = (Neg(l)) ? VarState::V_FALSE : VarState::V_TRUE;
    num_assigned++;
    dlevel[var] = dl;
}

void SolverState::before_add_clause(std::vector<int> &vector) {
    for (const auto &cb : cbs) cb->before_add_clause(vector);
}

void SolverState::after_initialize_clauses() {
    for (const auto &cb : cbs) {
        cb->after_initialize_clauses();
    }
}

void SolverState::after_assignment(Lit &l) {
    for (const auto &cb : cbs) cb->after_assignment(l);
}

SolverStatus SolverState::BCP() {
    before_bcp();
    auto res = _BCP();
    after_bcp();
    return res;
}

SolverStatus SolverState::_BCP() {
    while (qhead < trail.size()) {
        Lit NegatedLit = negate(trail[qhead++]);
        assert(lit_state(NegatedLit) == LitState::L_UNSAT);
        std::vector<int> new_watch_list; // The original watch list minus those clauses that changed a watch. The order is maintained.
        int new_watch_list_idx = watches[NegatedLit].size() -
                                 1; // Since we are traversing the watch_list backwards, this index goes down.
        new_watch_list.resize(watches[NegatedLit].size());
        for (auto clause_idx_it = watches[NegatedLit].rbegin();
             clause_idx_it != watches[NegatedLit].rend() && conflicting_clause_idx < 0; ++clause_idx_it) {
            Clause &c = cnf[*clause_idx_it];
            Lit l_watch = c.get_lw_lit(),
                    r_watch = c.get_rw_lit();
            bool binary = c.size() == 2;
            bool is_left_watch = (l_watch == NegatedLit);
            Lit other_watch = is_left_watch ? r_watch : l_watch;

            ClauseState res = ClauseState::C_UNDEF;
            int new_watch_index = next_not_false_index(c.literals, other_watch);
            if (new_watch_index != -1 and not binary) {
                c.switch_watches(is_left_watch, new_watch_index);
                auto new_lit = c.literals[new_watch_index];
                watches[new_lit].push_back(*clause_idx_it);
            } else res = clause_state_by_other_watch_state(other_watch);

            if (res != ClauseState::C_UNDEF)
                new_watch_list[new_watch_list_idx--] = *clause_idx_it; //in all cases but the move-watch_lit case we leave watch_lit where clause_idx_it is

            switch (res) {
                case ClauseState::C_SAT:
                    break; // nothing to do when clause has a satisfied literal.
                case ClauseState::C_UNIT: { // new implication
                    assign_literal(other_watch);
                    antecedent[l2v(other_watch)] = *clause_idx_it;
                    break;
                }
                case ClauseState::C_UNSAT: { // conflict
                    if (dl == 0) throw std::runtime_error("Unsat clause at DL=0");
                    conflicting_clause_idx = *clause_idx_it;
                    std::copy_backward(watches[NegatedLit].rend(), clause_idx_it, new_watch_list.end());
                    //TODO: TEST
                    break;
                }
            }
        }

        // resetting the list of clauses watched by this literal.
/*
        watches[NegatedLit].clear();
        new_watch_list_idx++; // just because of the redundant '--' at the end.
        watches[NegatedLit].insert(watches[NegatedLit].begin(), new_watch_list.begin() + new_watch_list_idx,
                                   new_watch_list.end());
*/
        new_watch_list_idx++; // just because of the redundant '--' at the end.
        watches[NegatedLit] = std::move(new_watch_list);
        watches[NegatedLit].erase(watches[NegatedLit].begin(), watches[NegatedLit].begin() + new_watch_list_idx);;
        new_watch_list.clear();
        if (conflicting_clause_idx >= 0) return SolverStatus::CONFLICT;
    }
    return SolverStatus::UNDEF;
}

void SolverState::before_backtrack() {
    for (const auto &cb : cbs) cb->before_backtrack();
}

void SolverState::backtrack(int k) {
    before_backtrack();
    _backtrack(k);
}

void SolverState::_backtrack(int dl_backtrack) {
    unassign_vars_in_trail_from_dl(dl_backtrack);

    //if (VarDecHeuristic == VAR_DEC_HEURISTIC::MINISAT) m_should_reset_iterators = true;
    trail.erase(trail.begin() + separators[dl_backtrack + 1], trail.end());
    qhead = trail.size();
    dl = dl_backtrack;

    /*
     * This part assigns the value of the "conflicting literal" (found in the analyze section).
     * to it's negated value, and sets it's value to .... TODO: WHY?
     * Should maybe take out into callback
    */
    assign_literal(assigned_lit);
    antecedent[l2v(assigned_lit)] = cnf.size() - 1;
    conflicting_clause_idx = -1;
}

void SolverState::unassign_vars_in_trail_from_dl(int dl_to_backtrack) {
    for (auto it = trail.begin() + separators[dl_to_backtrack + 1]; it != trail.end(); ++it) {
        Var v = l2v(*it);
        if (dlevel[v] == 0) continue;

        // we need the condition because of learnt unary clauses. In that case we enforce an assignment with dlevel = 0.
        var_state[v] = VarState::V_UNASSIGNED;
        num_assigned--;
        //if (VarDecHeuristic == VAR_DEC_HEURISTIC::MINISAT) m_curr_activity = max(m_curr_activity, m_activity[v]);
    }
}

bool SolverState::all_assigned() {
    return num_assigned == nvars;
}

void SolverState::validate_assignment() {
    check_all_variables_assigned();
    check_all_clauses_assigned();

}

void SolverState::check_all_variables_assigned() {
    for (auto i = 1; i <= nvars; i++) {
        if (var_state[i] == VarState::V_UNASSIGNED)
            throw std::runtime_error("Unassigned var: " + std::to_string(i));
    }
}

void SolverState::check_all_clauses_assigned() {
    //In all clauses, there is atleast one lit that's true
    for (auto &clause : cnf) {
        bool found = false;
        for (auto &lit : clause.literals) {
            if (lit_state(lit) == LitState::L_SAT) {
                found = true;
                break;
            }
        }
        if (!found) {
            throw std::runtime_error("Failed - No assigned lit in clause");
        }
    }

    // All unaries are assigned
    for (auto unary : unaries) {
        if (lit_state(unary) != LitState::L_SAT)
            throw std::runtime_error("Failed - Unary is not SAT");
    }
}

inline LitState SolverState::lit_state(Lit &l) {
    /*
     * This function returns if the literal was satisfied, unsatisfied or unassigned
     */
    VarState vs = var_state[l2v(l)];
    if (vs == VarState::V_UNASSIGNED) return LitState::L_UNASSIGNED; //If it's unassigned

    //If the variable was false, and the literal is negated, then it's value is true. Similarly for true.
    if ((Neg(l) and (vs == VarState::V_FALSE)) or (!Neg(l) and (vs == VarState::V_TRUE))) return LitState::L_SAT;
    else return LitState::L_UNSAT; //otherwise, unsat
}

void SolverState::write_assignment(std::string file_name) {
    std::ofstream out;
    out.open(file_name);
    out << "State: " << std::endl;
    for (auto i = 1; i <= nvars; i++) {
        auto var = var_state[i];
        char sign = (var == VarState::V_FALSE) ? -1 : var == VarState::V_TRUE ? 1 : 0;
        out << sign * i << " " << std::endl;
    }
}

int SolverState::next_not_false_index(Literals &literals, Lit &other_watch) {
    for (auto it = literals.begin(); it != literals.end(); ++it) {
        LitState LitState = lit_state(*it);
        if (LitState != LitState::L_UNSAT && *it != other_watch)  // found another watch_lit
            return distance(literals.begin(), it);
    }
    return -1; // didn't find another non false index
}

ClauseState SolverState::clause_state_by_other_watch_state(Lit &other_watch) {
    switch (lit_state(other_watch)) {
        case LitState::L_UNSAT: // conflict
            return ClauseState::C_UNSAT;
        case LitState::L_UNASSIGNED:
            return ClauseState::C_UNIT; // unit clause. Should assert the other watch_lit.
        case LitState::L_SAT:
            return ClauseState::C_SAT; // other literal is satisfied.
    }
    throw std::runtime_error("Should not reach here");
}

void SolverState::add_cb(std::shared_ptr<CallbackBase> cb) {
    cbs.push_back(cb);
}

void SolverState::before_assignment(Lit &l) {
    for (const auto &cb : cbs) cb->before_assignment(l);
}

void SolverState::decide(Lit l) {
    before_decide(l);
    dl++;
    if (dl > max_dl) {
        max_dl = dl;
        separators.push_back(trail.size());
    }
    else separators[dl] = trail.size();
    assign_literal(l);
    after_decide(l);
}

int SolverState::analyze() {
    Clause current_clause = cnf[conflicting_clause_idx];
    Literals new_literals;

    int resolve_num = 0,
            bktrk = 0,
            watch_lit = 0; // points to what literal in the learnt clause should be watched, other than the asserting one

    Lit u;
    Var v;
    do {
        //NEW CLAUSE ALL LITERALS FROM CURRENT LEVEL
        for (auto &lit : current_clause.literals) {
            v = l2v(lit);
            if (!marked[v]) {
                marked[v] = true;
                if (dlevel[v] == dl) ++resolve_num;
                else { // literals from previous decision levels (roots) are entered to the learned clause.
                    new_literals.push_back(lit);
                    int c_dl = dlevel[v];
                    if (c_dl > bktrk) {
                        bktrk = c_dl;
                        watch_lit = new_literals.size() - 1;
                    }
                }
            }
        }
        auto t_it = trail.rbegin();
        // LOOKS FOR FIRST MARKED CLAUSE
        while (t_it != trail.rend()) {
            u = *t_it;
            v = l2v(u);
            ++t_it;
            if (marked[v]) break;
        }
        marked[v] = false;
        --resolve_num;
        if (!resolve_num) continue;
        int ant = antecedent[v];
        current_clause = cnf[ant];
        current_clause.cl().erase(find(current_clause.cl().begin(), current_clause.cl().end(), u));
    } while (resolve_num > 0);
    for (auto &lit : new_literals)
        marked[l2v(lit)] = false;
    Lit Negated_u = negate(u);
    new_literals.push_back(Negated_u);
    assigned_lit = Negated_u;
    add_learned_clause(new_literals, watch_lit, new_literals.size() - 1);

    return bktrk;
}

void SolverState::before_learn_clause(std::vector<Lit> &learned) {
    for (const auto &cb : cbs) cb->before_learn_clause(learned);
}

void SolverState::after_learn_clause(std::vector<Lit> &learned) {
    for (const auto &cb : cbs) cb->after_learn_clause(learned);
}

void SolverState::_add_unary_clause(std::vector<int> &literals) {
    Lit l = literals.at(0);
    if ((Neg(l) == false) and (var_state.at(l2v(l)) == VarState::V_FALSE) and (dl == 0))
        throw std::runtime_error("UNSAT - Conflicting unaries for " + std::to_string(l2v(l)));

    unaries.push_back(l);
    if (var_state.at(l2v(l)) == VarState::V_UNASSIGNED) {
        assign_literal(l);
    }
}

void SolverState::add_learned_clause(Literals &literals, int lw, int rw) {
    before_learn_clause(literals);
    add_clause(literals, lw, rw);
    after_learn_clause(literals);
}

void SolverState::before_initialize_state() {
    for (const auto &cb : cbs) cb->before_initialize_state();
}

void SolverState::before_initialize_clauses(std::vector<std::vector<Lit>> &clauses) {
    for (const auto &cb : cbs) cb->before_initialize_clauses(clauses);
}

void SolverState::after_initialize_state() {
    for (const auto &cb : cbs) cb->after_initialize_state();
}

void SolverState::before_decide(Lit &l) {
    for (const auto &cb : cbs) cb->before_decide(l);
}

void SolverState::after_decide(Lit &l) {
    for (const auto &cb : cbs) cb->after_decide(l);
}

void SolverState::before_bcp() {
    for (const auto &cb : cbs) cb->before_bcp();
}

void SolverState::after_bcp() {
    for (const auto &cb : cbs) cb->after_bcp();
}

