//
// Created by matan on 23/01/2021.
//

#ifndef EDUSAT_VAR_H
#define EDUSAT_VAR_H


#include "src/utils/definitions.h"
#include <map>

class VarDecisionHeuristic {

};


class LR : public VarDecisionHeuristic {
    std::map<Lit, int> learned_clauses;
    std::map<Lit, int> participation_score;


    void decide();

    void after_decide(); //onAssign
    void after_analyze_conflict(); //onAnalyzeConflict
    void after_backtrack(); //onUnassign

};


class MiniSAT : public VarDecisionHeuristic {

};


#endif //EDUSAT_VAR_H
