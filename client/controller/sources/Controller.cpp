//
// Created by hubert on 03.12.15.
//

#include <typeinfo>
#include <pthread.h>
#include "../headers/Controller.h"

#include "../../../clientEvents/headers/NetworkEvent.h"
#include "../../../clientEvents/headers/ConfirmMessageEvent.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../strategy/headers/NetworkEventStrategy.h"
#include "../../strategy/headers/ConfirmMessageEventStrategy.h"
#include "../../strategy/headers/ChooseMenuOptionEventStrategy.h"
#include "../../../logger/easylogging++.h"

Controller::Controller(Model* model) : model(model) {
    initStrategyMap();
    networkController = NetworkController();
}

Controller::~Controller() {
    //TODO
}

void Controller::setView(View* view) {
    this->view = view;
}

void Controller::initStrategyMap() {
    strategyMap[typeid(NetworkEvent).name()] = new NetworkEventStrategy(this);
    strategyMap[typeid(ConfirmMessageEvent).name()] = new ConfirmMessageEventStrategy(this);
    strategyMap[typeid(ChooseMenuOptionEvent).name()] = new ChooseMenuOptionEventStrategy(this);
}

Model* Controller::getModel() {
    return model;
}

View* Controller::getView() {
    return view;
}

Queue<std::shared_ptr<BasicEvent>> *Controller::getEventsToServe() {
    return &eventsToServe;
}

void* Controller::threadStartHelper(void *param) {
    Controller* c = (Controller*)param;
    c->controllerWork();
}

void* Controller::controllerWork() {
    while(running){
        std::shared_ptr<BasicEvent> event = eventsToServe.pop();
        try {
            strategyMap.at(typeid(*event).name())->serveEvent(event.get());
        } catch (std::out_of_range &e) {
            LOG(ERROR) << "Bad type of incomming message";
        } catch (std::exception &e) {
            LOG(ERROR) << "Exception log: " << e.what();
        }
    }
}

void Controller::start() {
    pthread_create(&controllerThread, NULL, threadStartHelper, (void*)this);
    view->showMainMenu(model->getNotifications());
}

void Controller::exit() {
    //TODO
    //networkController.stop();
    running = false;
}