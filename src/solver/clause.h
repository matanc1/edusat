//
// Created by matan on 18/01/2021.
//

#ifndef EDUSAT_CLAUSE_H
#define EDUSAT_CLAUSE_H


#include <vector>
#include "src/utils/definitions.h"

class Clause {
    typedef std::vector<Lit> clause_t;
    clause_t c;
    int lw, rw; //watches;
public:
    Clause() {};

    void insert(int i) { c.push_back(i); }

    void lw_set(int i) { lw = i; /*assert(lw != rw);*/}

    void rw_set(int i) { rw = i; /*assert(lw != rw);*/}

    clause_t &cl() { return c; }

    int get_lw() { return lw; }

    int get_rw() { return rw; }

    int get_lw_lit() { return c[lw]; }

    int get_rw_lit() { return c[rw]; }

    int lit(int i) { return c[i]; }

    inline ClauseState next_not_false(bool is_left_watch, Lit other_watch, bool binary, int &loc);

    size_t size() { return c.size(); }

    void reset() { c.clear(); }

    void print() { for (clause_it it = c.begin(); it != c.end(); ++it) { cout << *it << " "; }; }

    void print_real_lits() {
        Lit l;
        cout << "(";
        for (clause_it it = c.begin(); it != c.end(); ++it) {
            l = l2rl(*it);
            cout << l << " ";
        }
        cout << ")";
    }

    void print_with_watches() {
        for (clause_it it = c.begin(); it != c.end(); ++it) {
            cout << l2rl(*it);
            int j = distance(c.begin(),
                             it); //also could write "int j = i - c.begin();"  : the '-' operator is overloaded to allow such things. but distance is more standard, as it works on all standard containers.
            if (j == lw) cout << "L";
            if (j == rw) cout << "R";
            cout << " ";
        };
    }
};


#endif //EDUSAT_CLAUSE_H
