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
private:
    Model* model;
    View* view;
    NetworkController networkController;
    Queue<std::shared_ptr<BasicEvent>> eventsToServe;
    Queue<std::shared_ptr<MessageWrapper>> sendQueue;
    std::map<std::string, BasicEventStrategy *> strategyMap;

    bool running = true;
    pthread_t controllerThread;
    void* controllerWork();
    static void* threadStartHelper(void* param);
public:
    Controller(Model* model);

    ~Controller();
    void setView(View* view);
    void initStrategyMap();
    void start();
    void exit();
    Model* getModel();
    View* getView();

    Queue<std::shared_ptr<BasicEvent>> *getEventsToServe();

    void sendMessage(std::shared_ptr<SimpleMessage> msg, std::string ip, int port);

};


#endif //RING_CONTROLLER_H
