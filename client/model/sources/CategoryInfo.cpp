//
// Created by hubert on 19.12.15.
//

#include <map>
#include "../headers/CategoryInfo.h"

using namespace std;

CategoryInfo::CategoryInfo() { }

CategoryInfo::CategoryInfo(const string &name, bool owner) : name(name), owner(owner) { }

const ConnectionInfo &CategoryInfo::getLeftNeighbour() const {
    return leftNeighbour;
}

void CategoryInfo::updateLeftNeighbour(const ConnectionInfo& info) {
    leftNeighbour = info;
}

void CategoryInfo::updateRightNeighbour(const ConnectionInfo& info) {
    rightNeighbour = info;
}

const ConnectionInfo &CategoryInfo::getRightNeighbour() const {
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

void CategoryInfo::addMessage(const string &message) {
    messages.push_back(message);
}

vector<string> CategoryInfo::getMessages() const {
    return messages;
}

const string &CategoryInfo::getName() const {
    return name;
}

bool CategoryInfo::isConfirmed() const {
    return confirmed;
}

void CategoryInfo::setConfirmed(bool confirmed) {
    this->confirmed = confirmed;
}

void CategoryInfo::addPendingUser(long userID, std::string userName) {
    pendingUsers.insert(pair<long, string>(userID, userName));
}

void CategoryInfo::removePendingUser(long userID) {
    pendingUsers.erase(userID);
}

std::map<long, string> CategoryInfo::getPendingUsers() const {
    return pendingUsers;
}