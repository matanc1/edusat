//
// Created by matan on 18/01/2021.
//

#include "options.h"


template<typename T> bool Tparse(string st, T val, T lb, T ub, T* var) {
    if (val < lb || val > ub)
        Abort(st + string(" value not in range"), 2);
    *var = val;
    return true;
}

void Abort(string s, int i) {
    std::cout << std::endl << "Abort: ";
    switch (i) {
        case 1: std::cout << "(input error)" << std::endl; break;
        case 2: std::cout << "command line arguments error" << std::endl; break;
        case 3: break;
        default: std::cout << "(exit code " << i << ")" << std::endl; break;
    }
    std::cout << s << std::endl;
    exit(i);
}

bool IntOption::parse(string st) {
    int val;
    try { val = stoi(st); }
    catch (...) {Abort("value " + st + " not numeric", 1); }
    return Tparse<int>(st, val, lb, ub, p_to_var);
}

