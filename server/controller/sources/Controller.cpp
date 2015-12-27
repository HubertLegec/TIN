#include "../headers/Controller.h"
#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../strategy/headers/CategoryManagementStrategy.h"
#include "../../strategy/headers/GetMessageStrategy.h"
#include <unistd.h>

Controller::Controller() : model(new Model) {
    initStrategyMap();
}

Controller::Controller(shared_ptr<Model> model) : model(model) {
    initStrategyMap();
}

void Controller::initStrategyMap() {
    strategyMap[typeid(GetMessage).name()] = new GetMessageStrategy(this);
    strategyMap[typeid(CategoryManagementMessage).name()] = new CategoryManagementStrategy(this);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void Controller::run() {
    shared_ptr<SimpleMessage> incomingMessage;

    while (true) {
        while (!incomingMessages.isEmpty()) {
            incomingMessage = incomingMessages.pop();
            strategyMap.at(typeid(*incomingMessage.get()).name())->serveEvent(incomingMessage.get());
        }

        usleep(INTERVAL_TIME);
    }
};
#pragma clang diagnostic pop