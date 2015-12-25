#include "../headers/Controller.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../strategy/headers/CategoryManagementStrategy.h"
#include "../../strategy/headers/GetMessageStrategy.h"

Controller::Controller() : model(new Model) {
    initStrategyMap();
}

Controller::Controller(shared_ptr<Model> model) : model(model) {
    initStrategyMap();
}

void Controller::initStrategyMap() {
    strategyMap[typeid(CategoryListMessage).name()] = GetMessageStrategy(this);
    strategyMap[typeid(CategoryManagementMessage).name()] = CategoryManagementStrategy(this);
}