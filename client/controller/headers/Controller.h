//
// Created by hubert on 03.12.15.
//
#ifndef RING_CONTROLLER_H
#define RING_CONTROLLER_H

#include <pthread.h>
#include <map>
#include <string>
#include "../../model/headers/Model.h"
#include "../../../networkModule/headers/NetworkController.h"
#include "../../../utils/Queue.hpp"
#include "../../../clientEvents/headers/BasicEvent.h"
#include "../../model/headers/Model.h"
#include "../../view/headers/View.h"
#include "../../strategy/headers/BasicEventStrategy.h"
#include "../../../networkModule/headers/MessageWrapper.h"

class BasicEventStrategy;
class View;

class Controller {
public:
    enum State {
        MAIN_MENU = 0,
        SIGN_UP = 1,
        CATEGORY_LIST = 2
    };
private:
    Model* model;
    View* view;
    NetworkController *networkController;
    Queue<std::shared_ptr<BasicEvent>> eventsToServe;
    Queue<std::shared_ptr<MessageWrapper>> sendQueue;
    Queue<std::shared_ptr<SimpleMessage>> receiveQueue;
    std::map<std::string, BasicEventStrategy *> strategyMap;
    pthread_t movingThread;

    State state;
    bool running = true;
    void* controllerWork();
    static void* threadStartHelper(void* param);

    void moveThreadWork();
public:
    Controller(Model* model);

    ~Controller();
    void setView(View* view);
    void initStrategyMap();
    void start();
    void exit();
    Model* getModel();
    View* getView();

    State getState() const;

    void setState(State state);

    Queue<std::shared_ptr<BasicEvent>> *getEventsToServe();

    Queue<std::shared_ptr<MessageWrapper>> *getSendQueue();

    void sendMessage(std::shared_ptr<SimpleMessage> msg, std::string ip, int port);

};


#endif //RING_CONTROLLER_H
