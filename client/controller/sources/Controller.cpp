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
#include "../../strategy/headers/NewMessageEventStrategy.h"
#include "../../strategy/headers/PendingUserEventStrategy.h"
#include "../../../networkMessage/headers/UserManagementMessage.h"

using namespace std;

Controller::Controller(Model *model) : model(model) {
    initStrategyMap();
}

Controller::~Controller() {
    delete networkController;
}

void Controller::setView(View *view) {
    this->view = view;
}

void Controller::initStrategyMap() {
    strategyMap["NETWORK_EVENT"] = new NetworkEventStrategy(this);
    strategyMap["CONFIRM_MESSAGE_EVENT"] = new ConfirmMessageEventStrategy(this);
    strategyMap["CHOOSE_MENU_OPTION_EVENT"] = new ChooseMenuOptionEventStrategy(this);
    strategyMap["CATEGORY_ACCESS_EVENT"] = new CategoryAccessEventStrategy(this);
    strategyMap["USER_ACCOUNT_EVENT"] = new UserAccountEventStrategy(this);
    strategyMap["NEW_MESSAGE_EVENT"] = new NewMessageEventStrategy(this);
    strategyMap["PENDING_USER_EVENT"] = new PendingUserEventStrategy(this);
}

Model *Controller::getModel() {
    return model;
}

View *Controller::getView() {
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
    pthread_create(&movingThread, NULL, threadStartHelper, this);
    networkController = new NetworkController(&sendQueue, &receiveQueue, model->getMyPort());
    state = MAIN_MENU;
    view->showMainMenu(model->getNotifications());
    while (running) {
        std::shared_ptr<BasicEvent> event = eventsToServe.pop();
        LOG(INFO) << "EVENT: " << event.get()->toString();
        try {
            BasicEventStrategy *str = strategyMap.at(event.get()->getName());
            str->serveEvent(event.get());
        } catch (out_of_range &e) {
            LOG(ERROR) << "Bad type of incomming message";
        } catch (exception &e) {
            LOG(ERROR) << "Exception log: " << e.what();
        }
    }
    LOG(INFO) << "Controller main loop closed";
}

void Controller::exit() {
    LOG(INFO) << "Controller::exit";
    //if user is registered send info about app close to server
    if (model->isRegistered()) {
        shared_ptr<UserManagementMessage> msg = shared_ptr<UserManagementMessage>(
                new UserManagementMessage(model->getUserId(), MessageType::CLIENT_CLOSE_APP));
        sendQueue.push(shared_ptr<MessageWrapper>(
                new MessageWrapper(msg, model->getServerInfo().getIP(), model->getServerInfo().getPort())));
    }
    networkController->stop();
}

void Controller::sendMessage(std::shared_ptr<SimpleMessage> msg, std::string ip, int port) {
    sendQueue.push(std::shared_ptr<MessageWrapper>(new MessageWrapper(msg, ip, port)));
}


void Controller::moveThreadWork() {
    LOG(INFO) << "Controller::moveThreadWork()";
    while (running) {
        shared_ptr<SimpleMessage> msg;
        msg = receiveQueue.pop();
        shared_ptr<NetworkEvent> event(new NetworkEvent(msg));
        eventsToServe.push(event);
        if (msg.get()->getMessageType() == MessageType::EXIT) {
            running = false;
        }
    }
    LOG(INFO) << "Controller::moveThreadWork() closed";
}

void *Controller::threadStartHelper(void *param) {
    Controller *c = (Controller *) param;
    c->moveThreadWork();
}

void Controller::incrementServerResponseNo() {
    pthread_mutex_lock(&serverResponseMutex);
    serverResponseNo++;
    pthread_mutex_unlock(&serverResponseMutex);
}

long Controller::getServerResponseNo() {
    long returnVal;
    pthread_mutex_lock(&serverResponseMutex);
    returnVal = serverResponseNo;
    pthread_mutex_unlock(&serverResponseMutex);
    return returnVal;
}

void *Controller::timeoutThread(void *param) {
    std::pair<Controller *, long> *arg = (std::pair<Controller *, long> *) param;
    Controller *c = arg->first;
    long responseNo = arg->second;

    usleep(2000000);
    if (responseNo == c->getServerResponseNo()) {
        c->getView()->showInfo("Timeout, failed to receive response from server.");
        c->getView()->showMainMenu(c->model->getNotifications());
    }

    delete arg;
}

void Controller::createTimeoutThread() {
    pthread_t t;
    std::pair<Controller *, long> *arg = new std::pair<Controller *, long>(this, getServerResponseNo());
    pthread_create(&t, nullptr, timeoutThread, arg);
}

void Controller::setRunning(bool running) {
    this->running = running;
}