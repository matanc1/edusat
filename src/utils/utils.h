//
// Created by matan on 21/01/2021.
//




#include <ctime>
#include "definitions.h"

#define Neg(l) (l & 1)

inline double cpuTime() {
    return (double) clock() / CLOCKS_PER_SEC;
}

inline unsigned int v2l(int i) { // maps a literal as it appears in the cnf to literal
    if (i < 0) return ((-i) << 1) - 1;
    else return i << 1;
}

inline Var l2v(Lit l) {
    return (l + 1) / 2;
}

inline Lit negate(Lit l) {
    if (Neg(l)) return l + 1;  // odd
    return l - 1;
}

inline int l2rl(int l) {
    return Neg(l) ? -((l + 1) / 2) : l / 2;
}
