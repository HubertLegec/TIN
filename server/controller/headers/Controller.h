#ifndef RING_CONTROLLER_H
#define RING_CONTROLLER_H

#include "../../model/headers/Model.h"

class Controller {
private:
    Model model;

public:
    Controller() { }

    Controller(Model model) : model(model) { }
};


#endif //RING_CONTROLLER_H
