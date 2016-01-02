//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_BASICEVENTSTRATEGY_H
#define RING_BASICEVENTSTRATEGY_H

#include "../../../clientEvents/headers/BasicEvent.h"
#include "../../controller/headers/Controller.h"
#include "../../model/headers/Model.h"
#include "../../view/headers/View.h"

class View;
class Controller;

class BasicEventStrategy {
protected:
    const std::string &getServerIP() const;

    int getServerPort() const;

    const std::string &getMyIP() const;

    int getMyPort() const;

    Model *getModel() const;

    View *getView() const;

    void sendMessage(std::shared_ptr<MessageWrapper> ptr) const;

    Controller* controller;
public:
    BasicEventStrategy() : controller(nullptr) { }

    BasicEventStrategy(Controller *controller) : controller(controller) { }

    void setController(Controller *controller);

    virtual void serveEvent(BasicEvent *event) = 0;
};


#endif //RING_BASICEVENTSTRATEGY_H
