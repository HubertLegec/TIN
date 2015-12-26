#ifndef RING_CONTROLLER_H
#define RING_CONTROLLER_H

#include <map>
#include <string>
#include "../../model/headers/Model.h"
#include "../../../networkMessage/headers/SimpleMessage.h"
#include "../../../utils/Queue.hpp"
#include "../../strategy/headers/BasicEventStrategy.h"

using namespace std;

class Controller {
private:
    const static unsigned INTERVAL_TIME = 100;
    map<string, BasicEventStrategy*> strategyMap;
    Queue<shared_ptr<SimpleMessage> > incomingMessages;
    Queue<shared_ptr<SimpleMessage> > outgoingMessages;
    shared_ptr<Model> model;

    void initStrategyMap();

public:
    Controller();

    Controller(shared_ptr<Model> model);

    shared_ptr<Model> getModel() const {
        return model;
    }

    void putOutgoingMessage(SimpleMessage &message) {
        outgoingMessages.push(shared_ptr<SimpleMessage>(&message));
    }

    void putOutgoingMessage(SimpleMessage *message) {
        outgoingMessages.push(shared_ptr<SimpleMessage>(message));
    }

    void putOutgoingMessage(shared_ptr<SimpleMessage> message) {
        outgoingMessages.push(message);
    }

    void putIncomingMessage(SimpleMessage &message) {
        incomingMessages.push(shared_ptr<SimpleMessage>(&message));
    }

    void putIncomingMessage(SimpleMessage *message) {
        incomingMessages.push(shared_ptr<SimpleMessage>(message));
    }

    void putIncomingMessage(shared_ptr<SimpleMessage> message) {
        incomingMessages.push(message);
    }

    const Queue<shared_ptr<SimpleMessage> > &getIncomingMessages() const {
        return incomingMessages;
    }

    const Queue<shared_ptr<SimpleMessage> > &getOutgoingMessages() const {
        return outgoingMessages;
    }

    const map<string, BasicEventStrategy*> &getStrategyMap() const {
        return strategyMap;
    }

    void run();
};


#endif //RING_CONTROLLER_H
