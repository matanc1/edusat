//
// Created by Matan on 15/02/2021.
//

#include "solver_state.h"
#include "src/utils/utils.h"


void SolverState::add_clause(std::vector<int> literals, int lw, int rw) {
    before_add_clause(literals);
    Clause c{literals};
    c.lw = lw;
    c.rw = rw;

    auto next_index = cnf.size();
    watches.at(c.literals[lw]).push_back(next_index);
    watches.at(c.literals[rw]).push_back(next_index);
    cnf.push_back(c);
}


void SolverState::initialize(unsigned int nvars, unsigned int nclauses, std::vector<std::vector<Lit>> clauses) {
    this->initialize_state(nvars, nclauses);
    this->initialize_clauses(clauses);
    after_initialization();
}

void SolverState::initialize_state(unsigned int nvars, unsigned int nclauses) {
    this->nvars = nvars;
    this->nclauses = nclauses;
    dl = 0;
    var_state.resize(nvars + 1, VarState::V_UNASSIGNED);
    dlevel.resize(nvars + 1);

    auto nlits = 2 * nvars;
    watches.resize(nlits + 1);
}

void SolverState::initialize_clauses(std::vector<std::vector<Lit>> clauses) {
    for (auto &c: clauses) {
        try {
            add_clause(c, 0, static_cast<int>(c.size()) - 1);
        }
        catch (const CancelException &e){
            continue;
        }
    }
}

SolverState::SolverState(Callbacks cbs) : cbs(cbs) {}

void SolverState::assign_literal(Lit l) {
    trail.push_back(l);
    auto var = l2v(l);
    var_state[var] = (Neg(l)) ? VarState::V_FALSE : VarState::V_TRUE;
    dlevel[var] = dl;

    after_assignment();
}

void SolverState::before_add_clause(std::vector<int> &vector) {
    for (const auto &cb : cbs) cb->before_add_clause(vector);
}

void SolverState::after_initialization() {
    for (const auto &cb : cbs) {
        cb->after_initialization();
    }
}

void SolverState::after_assignment() {
    for (const auto &cb : cbs) cb->after_assignment();
}

SolverStatus SolverState::BCP() {
/*
    while (qhead < trail.size()) {
        Lit NegatedLit = negate(trail[qhead++]);
        Assert(lit_state(NegatedLit) == LitState::L_UNSAT);
        std::vector<int> new_watch_list; // The original watch list minus those clauses that changed a watch. The order is maintained.
        int new_watch_list_idx = watches[NegatedLit].size() -
                                 1; // Since we are traversing the watch_list backwards, this index goes down.
        new_watch_list.resize(watches[NegatedLit].size());
        for (std::vector<int>::reverse_iterator it = watches[NegatedLit].rbegin();
             it != watches[NegatedLit].rend() && conflicting_clause_idx < 0; ++it) {
            Clause &c = cnf[*it];
            Lit l_watch = c.get_lw_lit(),
                    r_watch = c.get_rw_lit();
            bool binary = c.size() == 2;
            bool is_left_watch = (l_watch == NegatedLit);
            Lit other_watch = is_left_watch ? r_watch : l_watch;
            int NewWatchLocation;
            ClauseState res = c.next_not_false(is_left_watch, other_watch, binary, NewWatchLocation);
            if (res != ClauseState::C_UNDEF)
                new_watch_list[new_watch_list_idx--] = *it; //in all cases but the move-watch_lit case we leave watch_lit where it is
            switch (res) {
                case ClauseState::C_UNSAT: { // conflict
                    if (verbose_now()) print_state();
                    if (dl == 0) throw std::runtime_exception("UNSAT - Conflict at decision_level=0");
                    conflicting_clause_idx = *it;  // this will also break the loop
                    int dist = distance(it, watches[NegatedLit].rend()) -
                               1; // # of entries in watches[NegatedLit] that were not yet processed when we hit this conflict.
                    // Copying the remaining watched clauses:
                    for (int i = dist - 1; i >= 0; i--) {
                        new_watch_list[new_watch_list_idx--] = watches[NegatedLit][i];
                    }
                    if (verbose_now()) cout << "conflict" << endl;
                    break;
                }
                case ClauseState::C_SAT:
                    if (verbose_now()) cout << "clause is sat" << endl;
                    break; // nothing to do when clause has a satisfied literal.
                case ClauseState::C_UNIT: { // new implication
                    if (verbose_now()) cout << "propagating: ";
                    assert_lit(other_watch);
                    antecedent[l2v(other_watch)] = *it;
                    if (verbose_now()) cout << "new implication <- " << l2rl(other_watch) << endl;
                    break;
                }
                default: // replacing watch_lit
                    Assert(NewWatchLocation < static_cast<int>(c.size()));
                    int new_lit = c.lit(NewWatchLocation);
                    watches[new_lit].push_back(*it);
                    if (verbose_now()) {
                        c.print_real_lits();
                        cout << " now watched by " << l2rl(new_lit) << endl;
                    }
            }
        }
        // resetting the list of clauses watched by this literal.
        watches[NegatedLit].clear();
        new_watch_list_idx++; // just because of the redundant '--' at the end.
        watches[NegatedLit].insert(watches[NegatedLit].begin(), new_watch_list.begin() + new_watch_list_idx,
                                   new_watch_list.end());

        //print_watches();
        if (conflicting_clause_idx >= 0) return SolverState::CONFLICT;
        new_watch_list.clear();
    }
*/
    return SolverStatus::UNDEF;
}

void SolverState::before_backtrack() {
    for (const auto &cb : cbs) cb->before_backtrack();
}

void SolverState::backtrack(int) {
}
