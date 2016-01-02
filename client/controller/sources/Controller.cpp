//
// Created by hubert on 03.12.15.
//

#include <typeinfo>
#include "../headers/Controller.h"
#include "../../../logger/easylogging++.h"
#include "../../../clientEvents/headers/NetworkEvent.h"
#include "../../strategy/headers/NetworkEventStrategy.h"
#include "../../strategy/headers/ConfirmMessageEventStrategy.h"
#include "../../strategy/headers/ChooseMenuOptionEventStrategy.h"
#include "../../strategy/headers/CategoryAccessEventStrategy.h"
#include "../../strategy/headers/UserAccountEventStrategy.h"

using namespace std;

Controller::Controller(Model* model) : model(model) {
    initStrategyMap();
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
    strategyMap["CATEGORY_ACCESS_EVENT"] = new CategoryAccessEventStrategy(this);
    strategyMap["USER_ACCOUNT_EVENT"] = new UserAccountEventStrategy(this);
}

Model* Controller::getModel() {
    return model;
}

View* Controller::getView() {
    return view;
}

Controller::State Controller::getState() const {
    return state;
}

void Controller::setState(Controller::State state) {
    this->state = state;
}

Queue<std::shared_ptr<BasicEvent>> *Controller::getEventsToServe() {
    return &eventsToServe;
}

Queue<std::shared_ptr<MessageWrapper>> *Controller::getSendQueue() {
    return &sendQueue;
}

void Controller::start() {
    networkController = new NetworkController(&sendQueue, &receiveQueue, model->getMyPort());
    state = MAIN_MENU;
    view->showMainMenu(model->getNotifications());
    while(running){
        std::shared_ptr<BasicEvent> event = eventsToServe.pop();
        LOG(INFO) << "EVENT: " << event.get()->toString();
        try {
            strategyMap.at(event.get()->getName())->serveEvent(event.get());
        } catch (out_of_range &e) {
            LOG(ERROR) << "Bad type of incomming message";
        } catch (exception &e) {
            LOG(ERROR) << "Exception log: " << e.what();
        }
    }
}

void Controller::exit() {
    LOG(INFO) << "exit";
    receiveQueue.push(shared_ptr<SimpleMessage>(new SimpleMessage(MessageType::CLIENT_CLOSE_APP, model->getUserId())));
    networkController->stop();
    running = false;
}

void Controller::sendMessage(std::shared_ptr<SimpleMessage> msg, std::string ip, int port) {
    sendQueue.push(std::shared_ptr<MessageWrapper>(new MessageWrapper(msg, ip, port)));
}

void Controller::moveThreadWork() {
    while (running) {
        shared_ptr<SimpleMessage> msg;
        msg = receiveQueue.pop();
        shared_ptr<NetworkEvent> event(new NetworkEvent(msg));
        eventsToServe.push(event);
    }

}