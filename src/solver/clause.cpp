//
// Created by matan on 18/01/2021.
//

#include <cassert>
#include "clause.h"

void Clause::switch_watches(bool is_left_watch, int loc) {
    if (is_left_watch) lw = loc;
    else rw = loc;
}
