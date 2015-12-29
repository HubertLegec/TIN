//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_BASICEVENTSTRATEGY_H
#define RING_BASICEVENTSTRATEGY_H

#include "../../../clientEvents/headers/BasicEvent.h"
#include "../../controller/headers/Controller.h"
class Controller;

class BasicEventStrategy {
protected:
    Controller* controller;
public:
    BasicEventStrategy();
    BasicEventStrategy(Controller* controller);
    void setController(Controller* controller);

    virtual void serveEvent(BasicEvent *event) = 0;
};


#endif //RING_BASICEVENTSTRATEGY_H
