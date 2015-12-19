//
// Created by hubert on 03.12.15.
//

#include "../headers/Model.h"

Model::Model() { }

void Model::addJoinedCategory(long id, std::string name) {
    categoryNameIdMapping[name] = id;
    categories[id] = CategoryInfo(name);
}

void Model::addMessageToCategory(long categoryId, std::string message) {
    categories[categoryId].addMessage(message);
}

void Model::addMyCategory(long id, std::string name) {
    categoryNameIdMapping[name] = id;
    categories[id] = CategoryInfo(name, true);
}

std::vector<std::string> Model::getCategoryMessages(long categoryId) const {
    return categories[categoryId].getMessages();
}

std::string Model::getCategoryName(long id) const {
    return categories[id].getName();
}

long Model::getCategoryId(std::string name) const {
    return categoryNameIdMapping[name];
}

ConnectionInfo Model::getLeftNeighbour(long categoryId) const {
    return categories[categoryId].getLeftNeighbour();
}

void Model::updateLeftNeighbour(long categoryId, const ConnectionInfo& info) {
    categories[categoryId].updateLeftNeighbour(info);
}

ConnectionInfo Model::getRightNeighbour(long categoryId) const {
    return categories[categoryId].getRightNeighbour();
}

void Model::updateRightNeighbour(long categoryId, const ConnectionInfo &info) {
    categories[categoryId].updateRightNeighbour(info);
}

ConnectionInfo Model::getServerInfo() const {
    return serverInfo;
}

void Model::setServerInfo(std::string ip, int port) {
    serverInfo = ConnectionInfo(ip, port, "Server");
}

long Model::getUserId() const {
    return userID;
}

void Model::setUserId(long userId) {
    this->userID = userId;
}

std::string Model::getUserName() const {
    return userName;
}

void Model::setUserName(std::string userName) {
    this->userName = userName;
}

void Model::removeCategoryAndData(long id) {
    categoryNameIdMapping.erase(categories[id].getName());
    categories.erase(id);
}

std::vector<std::pair<long, std::string>> Model::getMyCategories() const {
    std::vector<std::pair<long, std::string>> result;
    for(std::pair<long, CategoryInfo> p : categories){
        if(p.second.isOwner()){
            result.push_back(std::pair<long, std::string>(p.first, p.second.getName()));
        }
    }
    return result;
}

std::vector<std::pair<long, std::string>> Model::getJoinedCategories() const {
    std::vector<std::pair<long, std::string>> result;
    for(std::pair<long, CategoryInfo> p : categories){
        if(!p.second.isOwner()){
            result.push_back(std::pair<long, std::string>(p.first, p.second.getName()));
        }
    }
    return result;
}

bool Model::isCategoryActive(long categoryId) const {
    return categories[categoryId].isActive();
}

void Model::setCategoryActive(long categoryId, bool active) {
    categories[categoryId].setActive(active);
}

void Model::addMessageToInbox(RingMessage message) {
    inbox.push_back(message);
}

std::vector<RingMessage> Model::getInboxMessages() const {
    return inbox;
}

void Model::markMessageAsRead(long messageIndex) {
    RingMessage msg = inbox[messageIndex];
    inbox.erase(inbox.begin() + messageIndex);
    categories[msg.getCategoryId()].addMessage(msg.getMsgText());
}