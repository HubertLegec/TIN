#ifndef RING_CONTROLLER_H
#define RING_CONTROLLER_H

#include <map>
#include <string>
#include "../../model/headers/Model.h"
#include "../../../networkMessage/headers/SimpleMessage.h"
#include "../../../utils/Queue.hpp"

using namespace std;

class BasicEventStrategy;

class Controller {
private:
    shared_ptr<Model> model;
    map<string, BasicEventStrategy> strategyMap;
    Queue<SimpleMessage> incomingMessages;
    Queue<SimpleMessage> outgoingMessages;

    void initStrategyMap();

public:
    Controller();

    Controller(shared_ptr<Model> model);

    shared_ptr<Model> getModel() const {
        return model;
    }

    void putOutgoingMessage(const SimpleMessage& message) {
        outgoingMessages.push(message);
    }

    const Queue<SimpleMessage> &getIncomingMessages() const {
        return incomingMessages;
    }

    const Queue<SimpleMessage> &getOutgoingMessages() const {
        return outgoingMessages;
    }
};


#endif //RING_CONTROLLER_H
