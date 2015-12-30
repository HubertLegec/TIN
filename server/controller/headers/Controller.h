#ifndef RING_CONTROLLER_H
#define RING_CONTROLLER_H

#include <map>
#include <string>
#include "../../model/headers/Model.h"
#include "../../../networkMessage/headers/SimpleMessage.h"
#include "../../../utils/Queue.hpp"
#include "../../strategy/headers/BasicEventStrategy.h"
#include "../../../networkModule/headers/MessageWrapper.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"

using namespace std;

enum {
    SERVER_ID = 0
};

class Controller {
private:
    const static unsigned INTERVAL_TIME = 200;
    map<string, BasicEventStrategy *> strategyMap;
    Queue<shared_ptr<SimpleMessage> > incomingMessages;
    Queue<shared_ptr<MessageWrapper> > outgoingMessages;
    shared_ptr<Model> model;

    void initStrategyMap();

public:
    Controller();

    Controller(shared_ptr<Model> model);

    shared_ptr<Model> getModel() const {
        return model;
    }

    void putOutgoingMessage(MessageWrapper &message) {
        outgoingMessages.push(shared_ptr<MessageWrapper>(&message));
    }

    void putOutgoingMessage(MessageWrapper *message) {
        outgoingMessages.push(shared_ptr<MessageWrapper>(message));
    }

    void putOutgoingMessage(shared_ptr<MessageWrapper> message) {
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

    void sendMessage(ServerInfoMessage *message);

    void sendMessage(SimpleMessage *message, const long userID);

    const Queue<shared_ptr<SimpleMessage> > &getIncomingMessages() const {
        return incomingMessages;
    }

    const Queue<shared_ptr<MessageWrapper> > &getOutgoingMessages() const {
        return outgoingMessages;
    }

    const map<string, BasicEventStrategy *> &getStrategyMap() const {
        return strategyMap;
    }

    void run();
};


#endif //RING_CONTROLLER_H
