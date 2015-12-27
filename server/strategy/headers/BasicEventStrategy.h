#ifndef RING_BASICEVENTSTRATEGY_H
#define RING_BASICEVENTSTRATEGY_H

#include "../../../networkMessage/headers/SimpleMessage.h"

class Controller;

class BasicEventStrategy {
protected:
    Controller *controller;

public:
    BasicEventStrategy();

    BasicEventStrategy(Controller *controller);

    void setController(Controller *controller);

    virtual void serveEvent(SimpleMessage *message) const = 0;
};


#endif //RING_BASICEVENTSTRATEGY_H
