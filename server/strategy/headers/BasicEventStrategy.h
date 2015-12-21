#ifndef RING_BASICEVENTSTRATEGY_H
#define RING_BASICEVENTSTRATEGY_H

#include "../../controller/headers/Controller.h"
#include "../../../networkMessage/headers/SimpleMessage.h"

class BasicEventStrategy {
protected:
    Controller *controller;

public:
    BasicEventStrategy() : controller(nullptr) { }

    BasicEventStrategy(Controller *controller) : controller(controller) { }

    void setController(Controller *controller) {
        this->controller = controller;
    }

    virtual void serveEvent(SimpleMessage *message) = 0;
};


#endif //RING_BASICEVENTSTRATEGY_H
