//
// Created by matan on 18/01/2021.
//

#ifndef EDUSAT_OPTIONS_H
#define EDUSAT_OPTIONS_H


#include <string>
#include <iostream>

using string = std::string;

class Option {
public:
    string msg;

    explicit Option(string msg) : msg(msg) {}

    virtual bool parse(string) = 0;

    virtual string val() = 0;
};


class IntOption : public Option {
public:
    IntOption(int *p, int _lb, int _ub, string _msg) : Option(_msg),
                                                       p_to_var(p), lb(_lb), ub(_ub) {};

    int *p_to_var; // pointer to the variable holding the option value.
    int lb; // lower-bound
    int ub; // upper-bound
    bool parse(string st);

    string val() override { return std::to_string(*p_to_var); }
};


class DoubleOption : public Option {
public:
    DoubleOption(double *p, double _lb, double _ub, string _msg): Option(_msg),
                                                                p_to_var(p), lb(_lb), ub(_ub) {};

    double *p_to_var; // pointer to the variable holding the option value.
    double lb; // lower-bound
    double ub; // upper-bound
    bool parse(string st);

    string val();
};


#endif //EDUSAT_OPTIONS_H
