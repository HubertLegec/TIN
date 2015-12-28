#include "../headers/Controller.h"
#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../strategy/headers/CategoryManagementStrategy.h"
#include "../../strategy/headers/GetMessageStrategy.h"
#include "../../../logger/easylogging++.h"
#include "../../../networkMessage/headers/UserManagementMessage.h"
#include "../../strategy/headers/UserManagementStrategy.h"
#include <unistd.h>

Controller::Controller() : model(new Model) {
    initStrategyMap();
}

Controller::Controller(shared_ptr<Model> model) : model(model) {
    initStrategyMap();
}

void Controller::initStrategyMap() {
    strategyMap[typeid(CategoryManagementMessage).name()] = new CategoryManagementStrategy(this);
    strategyMap[typeid(GetMessage).name()] = new GetMessageStrategy(this);
    strategyMap[typeid(UserManagementMessage).name()] = new UserManagementStrategy(this);
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
#pragma clang diagnostic pop