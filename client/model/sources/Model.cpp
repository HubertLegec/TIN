//
// Created by hubert on 03.12.15.
//

#include "../headers/Model.h"
#include "../../../logger/easylogging++.h"

using namespace std;

Model::Model() {
    userID = UNDEFINED_ID;
    myIP = CLIENT_DEFAULT_IP;
    myPort = CLIENT_DEFAULT_PORT;
}

void Model::addJoinedCategory(long id, const string &name) {
    categoryNameIdMapping.insert(pair<string, long>(name, id));
    CategoryInfo ci(name);
    ci.setConfirmed(false);
    ci.setActive(false);
    categories.insert(pair<long, CategoryInfo>(id, ci));
}

void Model::addMessageToCategory(long categoryId, const string &message) {
    categories.at(categoryId).addMessage(message);
}

void Model::addMyCategory(long id, const string &name) {
    categoryNameIdMapping.insert(pair<string, long>(name, id));
    CategoryInfo ci(name, true);
    ci.setConfirmed(true);
    ci.setActive(true);
    categories.insert(pair<long, CategoryInfo>(id, ci));
}

vector<string> Model::getCategoryMessages(long categoryId) {
    return categories.at(categoryId).getMessages();
}

const string &Model::getCategoryName(long id) const {
    return categories.at(id).getName();
}

long Model::getCategoryId(const string &name) const {
    return categoryNameIdMapping.at(name);
}

const ConnectionInfo &Model::getLeftNeighbour(long categoryId) {
    return categories.at(categoryId).getLeftNeighbour();
}

void Model::updateLeftNeighbour(long categoryId, const ConnectionInfo& info) {
    categories.at(categoryId).updateLeftNeighbour(info);
}

const ConnectionInfo &Model::getRightNeighbour(long categoryId) {
    return categories.at(categoryId).getRightNeighbour();
}

void Model::updateRightNeighbour(long categoryId, const ConnectionInfo &info) {
    categories.at(categoryId).updateRightNeighbour(info);
}

const ConnectionInfo &Model::getServerInfo() const {
    return serverInfo;
}

void Model::setServerInfo(const string &ip, int port) {
    serverInfo = ConnectionInfo(ip, port, "Server");
}

long Model::getUserId() const {
    return userID;
}

void Model::setUserId(long userId) {
    this->userID = userId;
}

const string &Model::getUserName() const {
    return userName;
}

void Model::setUserName(const string &userName) {
    this->userName = userName;
}

void Model::removeCategoryAndData(long id) {
    categoryNameIdMapping.erase(categories.at(id).getName());
    categories.erase(id);
}

map<long, string> Model::getMyCategories() const {
    map<long, string> result;
    for (auto p : categories) {
        if(p.second.isOwner()){
            result.insert(pair<long, string>(p.first, p.second.getName()));
        }
    }
    return result;
}

map<long, string> Model::getJoinedCategories() const {
    map<long, string> result;
    for (auto p : categories) {
        if (!p.second.isOwner() && p.second.isConfirmed()) {
            result.insert(pair<long, string>(p.first, p.second.getName()));
        }
    }
    return result;
}

bool Model::isCategoryActive(long categoryId) const {
    return categories.at(categoryId).isActive();
}

void Model::setCategoryActive(long categoryId, bool active) {
    categories.at(categoryId).setActive(active);
}

void Model::addMessageToInbox(const RingMessage &message) {
    inbox.push_back(message);
}

vector<RingMessage> Model::getInboxMessages() const {
    return inbox;
}

void Model::markMessageAsRead(long messageIndex) {
    RingMessage msg = inbox[messageIndex];
    inbox.erase(inbox.begin() + messageIndex);
    categories.at(msg.getCategoryId()).addMessage(msg.getMsgText());
}

void Model::addNotification(const string &notification) {
    notifications.push_back(notification);
}

vector<string> Model::getNotifications() {
    return notifications;
}

void Model::clearNotificationList() {
    notifications.clear();
}

void Model::setMyIP(const std::string &ip) {
    this->myIP = ip;
}

void Model::setMyPort(int port) {
    this->myPort = port;
}

const std::string &Model::getMyIP() const {
    return myIP;
}

int Model::getMyPort() const {
    return myPort;
}

bool Model::isRegistered() const {
    if (userID != UNDEFINED_ID) {
        return true;
    } else {
        return false;
    }
}

std::map<long, string> Model::getActiveCategories() const {
    map<long, string> result;
    for (auto c : categories) {
        if (c.second.isActive()) {
            result.insert(pair<long, string>(c.first, c.second.getName()));
        }
    }
    return result;
}

std::map<long, string> Model::getInactiveCategories() const {
    map<long, string> result;
    for (auto c : categories) {
        if (!c.second.isActive()) {
            result.insert(pair<long, string>(c.first, c.second.getName()));
        }
    }

    return result;
}

std::map<long, string> Model::getCategories() const {
    map<long, string> result;
    for (auto c : categories) {
        result.insert(pair<long, string>(c.first, c.second.getName()));
    }

    return result;
}

bool Model::isMyCategory(long categoryID) const {
    if (categories.find(categoryID) == categories.end()) {
        return false;
    } else {
        return true;
    }
}

void Model::confirmCategory(long categoryID) {
    categories.at(categoryID).setConfirmed(true);
    categories.at(categoryID).setActive(true);
}

void Model::addPendingUser(long categoryID, long userID, const std::string &userName) {
    categories.at(categoryID).addPendingUser(userID, userName);
}

void Model::removePendingUser(long categoryID, long userID) {
    categories.at(categoryID).removePendingUser(userID);
}

std::vector<PendingUserInfo> Model::getPendingUsers() const {
    vector<PendingUserInfo> result;
    for (auto c : categories) {
        if (c.second.isOwner()) {
            for (auto pendingUser : c.second.getPendingUsers()) {
                result.push_back(PendingUserInfo(c.first, pendingUser.first, pendingUser.second, c.second.getName()));
            }
        }
    }
    return result;
}

void Model::removeUserAccount() {
    categoryNameIdMapping.clear();
    inbox.clear();
    notifications.clear();
    categories.clear();
}