#include "../headers/Controller.h"
#include "../../strategy/headers/CategoryManagementStrategy.h"
#include "../../strategy/headers/GetMessageStrategy.h"
#include "../../../logger/easylogging++.h"
#include "../../strategy/headers/UserManagementStrategy.h"
#include "../../strategy/headers/ErrorMessageStrategy.h"

Controller::Controller(string ip, int port) : model(new Model), myIP(ip), myPort(port) {
    initStrategyMap();
    networkController.reset(new NetworkController(&outgoingMessages, &incomingMessages, myPort));
}

Controller::~Controller() {
    cleanUp();
}

void Controller::initStrategyMap() {
    strategyMap[IncomingMessageType::CATEGORY_MANAGEMENT] = new CategoryManagementStrategy(this);
    strategyMap[IncomingMessageType::GET_MESSAGE] = new GetMessageStrategy(this);
    strategyMap[IncomingMessageType::USER_MANAGEMENT] = new UserManagementStrategy(this);
    strategyMap[IncomingMessageType::ERROR_MESSAGE] = new ErrorMessageStrategy(this);
}

void Controller::sendMessage(ServerInfoMessage *message) {
    sendMessage(message, message->getExtraInfo());
}

void Controller::sendMessage(SimpleMessage *message, const long userID) {
    try {
        auto user = model->getUser(userID);
        auto wrapper = new MessageWrapper(shared_ptr<SimpleMessage>(message), user->getIP(), user->getPort());
        outgoingMessages.push(shared_ptr<MessageWrapper>(wrapper));
        LOG(INFO) << "Sent message to user " << user->getID();
    } catch (out_of_range &e) {
        LOG(DEBUG) << "Couldn't send message to user " << userID <<
        ". User doesn'y exist in the system";
    } catch (exception &e) {
        LOG(DEBUG) << "Couldn't send message to user " << userID <<
        ". Exception log: " << e.what();
    }
}

void Controller::sendMessage(SimpleMessage *message, string IP, int port) {
    try {
        auto wrapper = new MessageWrapper(shared_ptr<SimpleMessage>(message), IP, port);
        outgoingMessages.push(shared_ptr<MessageWrapper>(wrapper));
        LOG(INFO) << "Sent message to user " << IP;
    } catch (exception &e) {
        LOG(DEBUG) << "Couldn't send message to user " << IP <<
        ". Error log: " << e.what();
    }
}

#pragma clang diagnostic push

#pragma clang diagnostic ignored "-Wmissing-noreturn"

void Controller::run() {
    shared_ptr<SimpleMessage> incomingMessage;
    IncomingMessageType type;

    while (true) {
        incomingMessage = incomingMessages.pop();
        type = getMessageType(incomingMessage);
        if (type == IncomingMessageType::UNKNOWN) {
            LOG(ERROR) << "Bad type of incomming message";
        } else {
            try {
                strategyMap.at(type)->serveEvent(incomingMessage.get());
            } catch (exception &e) {
                LOG(DEBUG) << "Exception log: " << e.what();
            }
        }
    }
};

#pragma clang diagnostic popvoid

IncomingMessageType Controller::getMessageType(shared_ptr<SimpleMessage> message) {
    auto messageType = message->getMessageType();
    switch (messageType) {
        case GET:
            return IncomingMessageType::GET_MESSAGE;

        case CREATE_CATEGORY:
        case DESTROY_CATEGORY:
        case CATEGORY_LIST:
        case JOIN_CATEGORY:
        case LEFT_CATEGORY:
        case ACTIVATE_CATEGORY:
        case DEACTIVATE_CATEGORY:
            return IncomingMessageType::CATEGORY_MANAGEMENT;

        case CREATE_USER_ACCOUNT:
        case DELETE_USER_ACCOUNT:
        case CLIENT_CLOSE_APP:;
            return IncomingMessageType::USER_MANAGEMENT;

        case NETWORK_CONTROLLER_ERROR_MESSAGE:
            return ERROR_MESSAGE;

        default:
            return IncomingMessageType::UNKNOWN;
    }
}

void Controller::cleanUp() {
    networkController->stop();
}
