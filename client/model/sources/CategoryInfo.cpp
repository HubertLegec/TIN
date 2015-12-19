//
// Created by hubert on 19.12.15.
//

#include "../headers/CategoryInfo.h"

CategoryInfo::CategoryInfo() { }

CategoryInfo::CategoryInfo(std::string name, bool owner) : name(name), owner(owner) { }

ConnectionInfo CategoryInfo::getLeftNeighbour() const {
    return leftNeighbour;
}

void CategoryInfo::updateLeftNeighbour(const ConnectionInfo& info) {
    leftNeighbour = info;
}

void CategoryInfo::updateRightNeighbour(const ConnectionInfo& info) {
    rightNeighbour = info;
}

ConnectionInfo CategoryInfo::getRightNeighbour() const {
    return rightNeighbour;
}

bool CategoryInfo::isOwner() const {
    return owner;
}

void CategoryInfo::setOwner(bool owner) {
    this->owner = owner;
}

bool CategoryInfo::isActive() const {
    return active;
}

void CategoryInfo::setActive(bool active) {
    this->active = active;
}

void CategoryInfo::addMessage(const std::string& message) {
    messages.push_back(message);
}

std::vector<std::string> CategoryInfo::getMessages() const {
    return messages;
}

std::string CategoryInfo::getName() const {
    return name;
}
