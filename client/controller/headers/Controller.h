//
// Created by hubert on 03.12.15.
//
#ifndef RING_CONTROLLER_H
#define RING_CONTROLLER_H

#include <map>
#include <string>
#include "../../model/headers/Model.h"
//#include "../../../networkModule/headers/NetworkController.h"
#include "../../../utils/Queue.hpp"
#include "../../../clientEvents/headers/BasicEvent.h"
#include "../../model/headers/Model.h"
#include "../../view/headers/View.h"
#include "../../strategy/headers/BasicEventStrategy.h"

class BasicEventStrategy;
class View;

class Controller {
private:
    Model* model;
    View* view;
    //NetworkController networkController;
    Queue<BasicEvent> eventsToServe;
    std::map<std::string, BasicEventStrategy> strategyMap;

public:
    Controller(Model* model);
    void setView(View* view);
    void initStrategyMap();
    Model* getModel();
    View* getView();
    Queue<BasicEvent>* getEventsToServe();

};


#endif //RING_CONTROLLER_H
