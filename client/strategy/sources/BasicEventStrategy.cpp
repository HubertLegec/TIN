//
// Created by hubert on 30.12.15.
//

#include "../headers/BasicEventStrategy.h"

void BasicEventStrategy::setController(Controller *controller) {
    this->controller = controller;
}

View *BasicEventStrategy::getView() const {
    return controller->getView();
}

Model *BasicEventStrategy::getModel() const {
    return controller->getModel();
}

const std::string &BasicEventStrategy::getServerIP() const {
    return controller->getModel()->getServerInfo().getIP();
}

int BasicEventStrategy::getServerPort() const {
    return controller->getModel()->getServerInfo().getPort();
}

const std::string &BasicEventStrategy::getMyIP() const {
    return controller->getModel()->getMyIP();
}

int BasicEventStrategy::getMyPort() const {
    return controller->getModel()->getMyPort();
}

void BasicEventStrategy::sendMessage(std::shared_ptr<MessageWrapper> ptr) const {
    controller->getSendQueue()->push(ptr);
}

void BasicEventStrategy::showMainMenu() const {
    getView()->showMainMenu(getModel()->getNotifications());
    getModel()->clearNotificationList();
}