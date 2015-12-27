#include "../headers/BasicEventStrategy.h"
#include "../../controller/headers/Controller.h"

BasicEventStrategy::BasicEventStrategy() : controller(nullptr) { }

BasicEventStrategy::BasicEventStrategy(Controller *controller) : controller(controller) { }

void BasicEventStrategy::setController(Controller *controller) {
    this->controller = controller;
}