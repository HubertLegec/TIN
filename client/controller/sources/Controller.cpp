//
// Created by hubert on 03.12.15.
//

#include <typeinfo>
#include "../headers/Controller.h"

#include "../../../clientEvents/headers/NetworkEvent.h"
#include "../../../clientEvents/headers/ConfirmMessageEvent.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../strategy/headers/NetworkEventStrategy.h"
#include "../../strategy/headers/ConfirmMessageEventStrategy.h"
#include "../../strategy/headers/ChooseMenuOptionEventStrategy.h"

Controller::Controller(Model* model) : model(model) {
    initStrategyMap();
}

void Controller::setView(View* view) {
    this->view = view;
}

void Controller::initStrategyMap() {
    strategyMap[typeid(NetworkEvent).name()] = NetworkEventStrategy(this);
    strategyMap[typeid(ConfirmMessageEvent).name()] = ConfirmMessageEventStrategy(this);
    strategyMap[typeid(ChooseMenuOptionEvent).name()] = ChooseMenuOptionEventStrategy(this);
}

Model* Controller::getModel() {
    return model;
}

View* Controller::getView() {
    return view;
}

Queue<BasicEvent>* Controller::getEventsToServe() {
    return &eventsToServe;
}

void Controller::start() {
    pthread_create(&controllerThread, NULL, controllerWork, NULL);
}

void* Controller::controllerWork(void *param) {
    while(running){
        BasicEvent event = eventsToServe.pop();
        BasicEventStrategy strategy = strategyMap[typeid(event).name()];
        strategy.serveEvent(event);
    }
}

void Controller::exit() {
    //TODO
    //close connections
    running = false;
}