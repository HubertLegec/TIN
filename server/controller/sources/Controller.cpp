#include "../headers/Controller.h"
#include "../../strategy/headers/CategoryListMessageStrategy.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../strategy/headers/CategoryManagementStrategy.h"

Controller::Controller() : model(new Model) {
    initStrategyMap();
}

Controller::Controller(shared_ptr<Model> model) : model(model) {
    initStrategyMap();
}

void Controller::initStrategyMap() {
    strategyMap[typeid(CategoryListMessage).name()] = CategoryListMessageStrategy(this);
    strategyMap[typeid(CategoryManagementMessage).name()] = CategoryManagementStrategy(this);
}