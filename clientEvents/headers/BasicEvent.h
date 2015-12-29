//
// Created by hubert on 05.12.15.
//

#ifndef RING_BASICEVENT_H
#define RING_BASICEVENT_H

#include <string>

class BasicEvent {
public:
    virtual std::string toString() const = 0;

    virtual std::string getName() const = 0;
};


#endif //RING_BASICEVENT_H
