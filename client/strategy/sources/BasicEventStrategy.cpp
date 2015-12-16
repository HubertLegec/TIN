//
// Created by hubert.legec on 2015-12-15.
//

#include "../headers/BasicEventStrategy.h"

BasicEventStrategy::BasicEventStrategy() { }

BasicEventStrategy::BasicEventStrategy(Controller* controller) : controller(controller) {

}

void BasicEventStrategy::setController(Controller* controller) {
    this->controller = controller;
}

void BasicEventStrategy::serveMessage(BasicEvent event) {
    //child classes should implement this method
}