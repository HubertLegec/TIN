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

    void sendMessage(ServerInfoMessage *message);

    void sendMessage(SimpleMessage *message, const long userID);

    void sendMessage(shared_ptr<SimpleMessage> message, shared_ptr<User> user);

    void sendMessage(SimpleMessage *message, string IP, int port);

    void run();

    void cleanUp();
};


#endif //RING_CONTROLLER_H
