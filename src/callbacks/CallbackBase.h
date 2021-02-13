//
// Created by Matan on 14/02/2021.
//

#ifndef EDUSAT_CALLBACK_H
#define EDUSAT_CALLBACK_H


#include <memory>
#include <vector>

class CallbackBase {

public:
    virtual void before_bcp() {}
};

typedef std::vector<std::unique_ptr<CallbackBase>> Callbacks;

#endif //EDUSAT_CALLBACK_H
