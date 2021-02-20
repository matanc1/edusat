//
// Created by Matan on 15/02/2021.
//

#ifndef EDUSAT_SOLVER_STATE_H
#define EDUSAT_SOLVER_STATE_H

#include <vector>
#include "src/utils/definitions.h"
#include "clause.h"
#include "src/callbacks/CallbackBase.h"

class SolverState {

    /* Initialization */
    void initialize_clauses(std::vector<std::vector<Lit>> clauses);

    void initialize_state(unsigned int nvars, unsigned int nclauses);

    /* Callbacks */
    void before_add_clause(std::vector<int> &vector);

    void after_initialize_clauses();

    void before_assignment(Lit &l);
    void after_assignment(Lit &l);

    void before_decide(Lit &l);
    void after_decide(Lit &l);
    void before_backtrack();
    void before_learn_clause(std::vector<Lit> &learned);
    void after_learn_clause(std::vector<Lit> &learned);

    /* DPLL helpers */
    void _backtrack(int k);

    void unassign_vars_in_trail_from_dl(int dl_to_backtrack);

    void _assign_literal(Lit l);
    int next_not_false_index(Literals &vector, Lit &watch);

    /* Validation */
    void check_all_clauses_assigned();

    void check_all_variables_assigned();

public:
    std::vector<Clause> cnf;
    std::vector<std::vector<int>> watches;  // Lit => vector of clause indices into CNF
    std::vector<VarState> var_state;  // current assignment
    std::vector<Lit> unaries;
    std::vector<int> dlevel; // var => decision level in which this variable was assigned its value.
    std::vector<Lit> trail; // assignment stack
    std::vector<int> separators; // indices into trail showing increase in dl
    std::vector<int> antecedent; // var => clause index in the cnf vector. For variables that their value was assigned in BCP, this is the clause that gave this variable its value.
    std::vector<bool> marked;	// var => seen during analyze()
    unsigned int nclauses;
    unsigned int nvars;
    unsigned int qhead = 0;
    int num_assigned; // The number of currently assigned variables
    int conflicting_clause_idx; // holds the index of the current conflicting clause in cnf[]. -1 if none.
    Lit assigned_lit;

    int dl; // current decision level
    Callbacks cbs;

    SolverState(Callbacks cbs);

    void initialize(unsigned int nvars, unsigned int nclauses, std::vector<Literals> clauses);

    void add_clause(std::vector<int> dimacs_clauses, int lw, int rw);

    void assign_literal(Lit l);

    SolverStatus BCP();

    void backtrack(int k);

    bool all_assigned();

    void validate_assignment();

    LitState lit_state(Lit &lit);

    void write_assignment(std::string file_name);

    ClauseState clause_state_by_other_watch_state(Lit &other_watch);

    void add_cb(std::shared_ptr<CallbackBase> cb);
    void decide(Lit l);
    int analyze();

    void _add_clause(const std::vector<int> &literals, int lw, int rw);

    void _add_unary_clause(std::vector<int> &literals);

    void add_learned_clause(Literals &literals, int lw, int rw);

    void before_initialize_state();

    void before_initialize_clauses(std::vector<std::vector<Lit>> &clauses);

    void after_initialize_state();

    SolverStatus _BCP();

    void before_bcp();
    void after_bcp();

    int max_dl;

    bool is_clause_sat(Clause &clause);

    bool all_lits_assigned(Clause &clause);

    void add_output(std::map<std::string, std::string> &output);
};


#endif //EDUSAT_SOLVER_STATE_H
