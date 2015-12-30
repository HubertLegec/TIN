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
    const std::string &getServerIP() const {
        return controller->getModel()->getServerInfo().getIP();
    }

    int getServerPort() const {
        return controller->getModel()->getServerInfo().getPort();
    }

    const std::string &getMyIP() const {
        return controller->getModel()->getMyIP();
    }

    int getMyPort() const {
        return controller->getModel()->getMyPort();
    }

    Model *getModel() const {
        return controller->getModel();
    }

    View *getView() const {
        return controller->getView();
    }

    void sendMessage(std::shared_ptr<MessageWrapper> ptr) const {
        controller->getSendQueue()->push(ptr);
    }

    Controller* controller;
public:
    BasicEventStrategy() : controller(nullptr) { }

    BasicEventStrategy(Controller *controller) : controller(controller) { }

    virtual void serveEvent(BasicEvent *event) = 0;
};


#endif //RING_BASICEVENTSTRATEGY_H
