//
// Created by Matan on 30/01/2021.
//

#ifndef EDUSAT_DEFINITIONS_H
#define EDUSAT_DEFINITIONS_H

#include <vector>

typedef int Var;
typedef int Lit;


typedef std::vector<Lit> Literals;

enum class SolverStatus {
    UNSAT,
    SAT,
    CONFLICT,
    UNDEF,
    TIMEOUT
};

enum class LitState {
    L_UNSAT,
    L_SAT,
    L_UNASSIGNED
};

enum class VarState {
    V_FALSE,
    V_TRUE,
    V_UNASSIGNED
};

enum class ClauseState {
    C_UNSAT,
    C_SAT,
    C_UNIT,
    C_UNDEF
};

#endif //EDUSAT_DEFINITIONS_H
