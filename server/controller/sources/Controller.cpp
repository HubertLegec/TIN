#include "../headers/Controller.h"
#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../strategy/headers/CategoryManagementStrategy.h"
#include "../../strategy/headers/GetMessageStrategy.h"
#include "../../../logger/easylogging++.h"
#include "../../../networkMessage/headers/UserManagementMessage.h"
#include "../../strategy/headers/UserManagementStrategy.h"

Controller::Controller() : model(new Model), myIP(DEFAULT_IP), myPort(DEFAULT_PORT) {
    initStrategyMap();
    networkController.reset(new NetworkController(&outgoingMessages, &incomingMessages, myPort));
}

Controller::Controller(shared_ptr<Model> model) : model(model), myIP(DEFAULT_IP), myPort(DEFAULT_PORT) {
    initStrategyMap();
    networkController.reset(new NetworkController(&outgoingMessages, &incomingMessages, myPort));
}

Controller::Controller(shared_ptr<Model> model, int port) : model(model), myIP(DEFAULT_IP), myPort(port) {
    initStrategyMap();
    networkController.reset(new NetworkController(&outgoingMessages, &incomingMessages, myPort));
}

Controller::Controller(shared_ptr<Model> model, string ip, int port) : model(model), myIP(ip), myPort(port) {
    initStrategyMap();
    networkController.reset(new NetworkController(&outgoingMessages, &incomingMessages, myPort));
}

void Controller::initStrategyMap() {
    strategyMap[typeid(CategoryManagementMessage).name()] = new CategoryManagementStrategy(this);
    strategyMap[typeid(GetMessage).name()] = new GetMessageStrategy(this);
    strategyMap[typeid(UserManagementMessage).name()] = new UserManagementStrategy(this);
}

void Controller::sendMessage(ServerInfoMessage *message) {
    sendMessage(message, message->getExtraInfo());
}

void Controller::sendMessage(SimpleMessage *message, const long userID) {
    try {
        auto user = model->getUser(userID);
        auto wrapper = new MessageWrapper(shared_ptr<SimpleMessage>(message), user->getIP(), user->getPort());
        outgoingMessages.push(shared_ptr<MessageWrapper>(wrapper));
        LOG(DEBUG) << "Sent message to user " << message->getSenderID();
    } catch (out_of_range &e) {
        LOG(ERROR) << "Couldn't send message to user " << message->getSenderID() <<
        ". User doesn'y exist in the system";
    } catch (exception &e) {
        LOG(ERROR) << "Couldn't send message to user " << message->getSenderID() <<
        ". Error log: " << e.what();
    }
}

#pragma clang diagnostic push

#pragma clang diagnostic ignored "-Wmissing-noreturn"

void Controller::run() {
    shared_ptr<SimpleMessage> incomingMessage;

    while (true) {
        while (!incomingMessages.isEmpty()) {
            incomingMessage = incomingMessages.pop();
            try {
                strategyMap.at(typeid(*incomingMessage).name())->serveEvent(incomingMessage.get());
            } catch (out_of_range &e) {
                LOG(ERROR) << "Bad type of incomming message";
            } catch (exception &e) {
                LOG(ERROR) << "Exception log: " << e.what();
            }
        }

        usleep(INTERVAL_TIME);
    }
};

#pragma clang diagnostic popvoid
