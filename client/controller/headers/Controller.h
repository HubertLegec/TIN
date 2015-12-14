//
// Created by hubert on 03.12.15.
//
#ifndef RING_CONTROLLER_H
#define RING_CONTROLLER_H

#include "Model.h"
#include "../../../networkModule/headers/NetworkController.hpp"
#include "../../../utils/Queue.hpp"
#include "../../../clientEvents/headers/BasicEvent.h"
#include "../../view/headers/View.h"

class Controller {
private:
    Model model;
    View view;
    NetworkController networkController;
    Queue<BasicEvent> eventsToServe;

public:
    Controller(Model model);
    void setView(View view);

};


#endif //RING_CONTROLLER_H
