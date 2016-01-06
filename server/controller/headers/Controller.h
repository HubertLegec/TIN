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
#include "../../../networkModule/headers/NetworkController.h"

using namespace std;

enum {
    SERVER_ID = 0
};

enum IncomingMessageType {
    CATEGORY_MANAGEMENT,
    GET_MESSAGE,
    USER_MANAGEMENT,
    ERROR_MESSAGE,
    EXIT_MESSAGE,
    UNKNOWN
};

enum WorkingStatus {
    NORMAL,
    WAITING_TO_EXIT
};

class Controller {
private:
    int myPort;
    string myIP;
    WorkingStatus workingStatus;
    map<IncomingMessageType, BasicEventStrategy *> strategyMap;
    Queue<shared_ptr<SimpleMessage> > incomingMessages;
    Queue<shared_ptr<MessageWrapper> > outgoingMessages;
    shared_ptr<Model> model;
    shared_ptr<NetworkController> networkController;

    IncomingMessageType getMessageType(shared_ptr<SimpleMessage> shared_ptr);

    void initStrategyMap();

public:
    Controller(string ip, int port);

    ~Controller();

    shared_ptr<Model> getModel() const {
        return model;
    }

    int getMyPort() const {
        return myPort;
    }

    const string &getMyIP() const {
        return myIP;
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

    void sendMessage(shared_ptr<SimpleMessage> message, shared_ptr<User> user);

    void sendMessage(SimpleMessage *message, string IP, int port);

    const Queue<shared_ptr<SimpleMessage> > &getIncomingMessages() const {
        return incomingMessages;
    }

    const Queue<shared_ptr<MessageWrapper> > &getOutgoingMessages() const {
        return outgoingMessages;
    }

    const map<IncomingMessageType, BasicEventStrategy *> &getStrategyMap() const {
        return strategyMap;
    }

    void run();

    void cleanUp();

    void setWorkingStatus(const WorkingStatus &workingStatus) {
        Controller::workingStatus = workingStatus;
    }

    const WorkingStatus &getWorkingStatus() const {
        return workingStatus;
    }
};


#endif //RING_CONTROLLER_H
