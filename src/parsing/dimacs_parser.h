//
// Created by matan on 19/01/2021.
//

#ifndef EDUSAT_DIMACS_PARSER_H
#define EDUSAT_DIMACS_PARSER_H

#include <fstream>
#include <vector>
#include <src/solver/solver.h>

namespace dimacs_parser {
    void skip_headers(std::ifstream &input);

    std::tuple<int, int> read_problem_definition(std::ifstream &input);

    std::vector<Clause> read_clauses(std::ifstream &input);
}


#endif //EDUSAT_DIMACS_PARSER_H
