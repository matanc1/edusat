//
// Created by matan on 18/01/2021.
//

#ifndef EDUSAT_CLAUSE_H
#define EDUSAT_CLAUSE_H


#include <vector>
#include "src/utils/definitions.h"
#include <ostream>



class Clause {

public:
    Literals literals;
    int lw, rw; //watches;

    Clause() = default;

    Clause(Literals c) : literals{std::move(c)} {
        lw = 0;
        rw = 1;
    }

    int get_lw_lit() { return literals[lw]; }

    int get_rw_lit() { return literals[rw]; }

    int lit(int i) { return literals[i]; }

    size_t size() { return literals.size(); }

    void switch_watches(bool is_left_watch, int loc);
};

#endif //EDUSAT_CLAUSE_H
