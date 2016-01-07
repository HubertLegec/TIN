#include "../headers/Controller.h"
#include "../../strategy/headers/CategoryManagementStrategy.h"
#include "../../strategy/headers/GetMessageStrategy.h"
#include "../../../logger/easylogging++.h"
#include "../../strategy/headers/UserManagementStrategy.h"
#include "../../strategy/headers/ErrorMessageStrategy.h"
#include "../../utils/ServerGlobalConstants.h"

Controller::Controller(string ip, int port) : model(new Model), myIP(ip), myPort(port), workingStatus(NORMAL) {
    initStrategyMap();
    networkController.reset(new NetworkController(&outgoingMessages, &incomingMessages, myIP, myPort));
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
        sendMessage(shared_ptr<SimpleMessage>(message), user);
    } catch (out_of_range &e) {
        LOG(DEBUG) << "Couldn't send message to user " << userID <<
        ". User doesn'y exist in the system";
    }
}

void Controller::sendMessage(shared_ptr<SimpleMessage> message, shared_ptr<User> user) {
    try {
        auto wrapper = new MessageWrapper(message, user->getIP(), user->getPort());
        outgoingMessages.push(shared_ptr<MessageWrapper>(wrapper));
        LOG(INFO) << "Sent message to user " << user->getID();
    } catch (exception &e) {
        LOG(DEBUG) << "Couldn't send message to user " << user->getID() <<
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
            LOG(INFO) << "Bad type of incomming message";
        } else if (type == IncomingMessageType::EXIT_MESSAGE) {
            LOG(INFO) << "Get EXIT_MESSAGE. Closing server app!";
            exit(ServerGlobalConstant::EXIT_MESSAGE_CODE);
        } else if (workingStatus == WorkingStatus::NORMAL) {
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
        case MessageType::GET:
            return IncomingMessageType::GET_MESSAGE;

        case MessageType::CREATE_CATEGORY:
        case MessageType::DESTROY_CATEGORY:
        case MessageType::CATEGORY_LIST:
        case MessageType::JOIN_CATEGORY:
        case MessageType::LEFT_CATEGORY:
        case MessageType::ACTIVATE_CATEGORY:
        case MessageType::DEACTIVATE_CATEGORY:
        case MessageType::NEW_MEMBER_CONFIRM:
        case MessageType::NEW_MEMBER_REJECT:
            return IncomingMessageType::CATEGORY_MANAGEMENT;

        case MessageType::CREATE_USER_ACCOUNT:
        case MessageType::DELETE_USER_ACCOUNT:
        case MessageType::CLIENT_CLOSE_APP:;
            return IncomingMessageType::USER_MANAGEMENT;

        case MessageType::NETWORK_CONTROLLER_ERROR_MESSAGE:
            return IncomingMessageType::ERROR_MESSAGE;

        case MessageType::EXIT:
            return IncomingMessageType::EXIT_MESSAGE;

        default:
            return IncomingMessageType::UNKNOWN;
    }
}

void Controller::cleanUp() {
    workingStatus = WorkingStatus::WAITING_TO_EXIT;
    networkController->stop();
}