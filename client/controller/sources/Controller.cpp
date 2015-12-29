//
// Created by hubert on 03.12.15.
//

#include <typeinfo>
#include "../headers/Controller.h"
#include "../../../logger/easylogging++.h"
#include "../../strategy/headers/NetworkEventStrategy.h"
#include "../../strategy/headers/ConfirmMessageEventStrategy.h"
#include "../../strategy/headers/ChooseMenuOptionEventStrategy.h"

Controller::Controller(Model* model) : model(model) {
    initStrategyMap();
    networkController = new NetworkController();
}

Controller::~Controller() {
    delete networkController;
}

void Controller::setView(View* view) {
    this->view = view;
}

void Controller::initStrategyMap() {
    strategyMap["NETWORK_EVENT"] = new NetworkEventStrategy(this);
    strategyMap["CONFIRMATION_MESSAGE_EVENT"] = new ConfirmMessageEventStrategy(this);
    strategyMap["CHOOSE_MENU_OPTION_EVENT"] = new ChooseMenuOptionEventStrategy(this);
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

void Controller::start() {
    view->showMainMenu(model->getNotifications());
    while(running){
        std::shared_ptr<BasicEvent> event = eventsToServe.pop();
        LOG(INFO) << "EVENT: " << event.get()->toString();
        try {
            strategyMap.at(event.get()->getName())->serveEvent(event.get());
        } catch (std::out_of_range &e) {
            LOG(ERROR) << "Bad type of incomming message";
        } catch (std::exception &e) {
            LOG(ERROR) << "Exception log: " << e.what();
        }
    }
}

void Controller::exit() {
    LOG(INFO) << "exit";
    //TODO
    //networkController.stop();
    running = false;
}

void Controller::sendMessage(std::shared_ptr<SimpleMessage> msg, std::string ip, int port) {
    sendQueue.push(std::shared_ptr<MessageWrapper>(new MessageWrapper(msg, ip, port)));
}