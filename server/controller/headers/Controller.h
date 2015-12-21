#ifndef RING_CONTROLLER_H
#define RING_CONTROLLER_H

#include "../../model/headers/Model.h"

class Controller {
private:
    shared_ptr<Model> model;

public:
    Controller() { }

    Controller(shared_ptr<Model> model) : model(model) { }

    shared_ptr<Model> getModel() const {
        return model;
    }
};


#endif //RING_CONTROLLER_H
