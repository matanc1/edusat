//
// Created by Matan on 23/02/2021.
//

#ifndef EDUSAT_SATSOLVEREXCEPTION_H
#define EDUSAT_SATSOLVEREXCEPTION_H
#include <exception>
#include <string>
#include <map>

class SatSolverException : std::exception{
private:
    std::string message;

public:
    explicit SatSolverException(std::string message) : message(message) {};

    virtual const char *what() const throw() {
        return message.c_str();
    }

    virtual void add_output(std::map<std::string, std::string> &output) {
        output["error"] = what();
        output["status"] = "ERROR";
    }
};


class UnsatException : public SatSolverException {

public:
    explicit UnsatException(std::string message) : SatSolverException(message) {};

    virtual void add_output(std::map<std::string, std::string> &output) {
        output["error"] = what();
        output["status"] = "UNSAT";
    }
};


#endif //EDUSAT_SATSOLVEREXCEPTION_H
