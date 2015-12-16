//
// Created by hubert on 03.12.15.
//

#include <typeinfo>
#include "../headers/Controller.h"

#include "../../../clientEvents/headers/NetworkEvent.h"
#include "../../../clientEvents/headers/ConfirmMessageEvent.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../strategy/headers/NetworkEventStragegy.h"
#include "../../strategy/headers/ConfirmMessageEventStrategy.h"
#include "../../strategy/headers/ChooseMenuOptionEventStrategy.h"

Controller::Controller(Model* model) : model(model) {
    initStrategyMap();
}

void Controller::setView(View* view) {
    this->view = view;
}

void Controller::initStrategyMap() {
    strategyMap[typeid(NetworkEvent).name()] = NetworkEventStragegy(this);
    strategyMap[typeid(ConfirmMessageEvent).name()] = ConfirmMessageEventStrategy(this);
    strategyMap[typeid(ChooseMenuOptionEvent).name()] = ChooseMenuOptionEventStrategy(this);
}