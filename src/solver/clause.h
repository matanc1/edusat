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

    void insert(int i) { literals.push_back(i); }

    void lw_set(int i) { lw = i; /*assert(lw != rw);*/}

    void rw_set(int i) { rw = i; /*assert(lw != rw);*/}

    Literals &cl() { return literals; }

    int get_lw() { return lw; }

    int get_rw() { return rw; }

    int get_lw_lit() { return literals[lw]; }

    int get_rw_lit() { return literals[rw]; }

    int lit(int i) { return literals[i]; }

    inline ClauseState next_not_false(bool is_left_watch, Lit other_watch, bool binary, int &loc);

    size_t size() { return literals.size(); }

    void reset() { literals.clear(); }
};

#endif //EDUSAT_CLAUSE_H
