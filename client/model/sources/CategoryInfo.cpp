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

void CategoryInfo::addMessage(std::string Message) {
    messages.push_back(messages);
}

std::vector<std::string> CategoryInfo::getMessages() const {
    return messages;
}

std::string CategoryInfo::getName() const {
    return name;
}