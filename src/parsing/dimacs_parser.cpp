//
// Created by matan on 19/01/2021.
//

#include "dimacs_parser.h"
#include <limits>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <tuple>
#include <set>
#include <src/solver/exceptions.h>

using namespace std;

void dimacs_parser::skip_headers(ifstream &input) {
    //Ignore "max" characters until reaching an escape character
    while (input.peek() == 'c') input.ignore(numeric_limits<streamsize>::max(), '\n');
}

vector<string> split_line_to_words(string &line) {
    istringstream iss(line);
    auto iter = istream_iterator<string>(iss);
    vector<string> vec;
    move(iter, istream_iterator<string>(), back_inserter(vec));
    return vec; //COPY ELISION?
}

tuple<int, int> dimacs_parser::read_problem_definition(ifstream &input) {
    string line;
    getline(input, line);
    auto line_words = split_line_to_words(line);
    int nvars = stoi(line_words[2]);
    int nclauses = stoi(line_words[3]);
    return make_pair(nvars, nclauses);
}

vector<int> create_clause(std::string str) {
    int t;
    set<int> terms;
    stringstream ss(str);

    while (ss >> t) {
        if (t != 0) terms.insert(t);
        else if (terms.size() == 0) throw SatSolverException("A clause can't be empty.");
    }

    return vector(terms.begin(), terms.end());
}

std::vector<std::vector<int>> dimacs_parser::read_clauses(ifstream &input) {
    string line;
    vector<string> lines;
    vector<vector<int>> clauses;

    while (getline(input, line)) lines.push_back(std::move(line));
    for (auto &line : lines) {
        clauses.push_back(create_clause(line));
    }

    return clauses;
}

