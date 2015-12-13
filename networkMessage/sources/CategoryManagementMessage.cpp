//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/CategoryManagementMessage.h"

CategoryManagementMessage::CategoryManagementMessage() { }

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, long categoryID , const std::string& userName)
                                                    : SimpleMessage(type, senderID) {
    this->categoryID = categoryID;
    this->userID = -1;
    this->userName = userName;

    this->size = SimpleMessage::getMessageSize() + 4*sizeof(long) + userName.size();
}

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, long categoryID, long userID)
                                                    : SimpleMessage(type, senderID){
    this->categoryID = categoryID;
    this->userID = userID;

    this->size = SimpleMessage::getMessageSize() + 4*sizeof(long);
}

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, const std::string &categoryName,
                                                     const std::string &userName) : SimpleMessage(type, senderID) {
    this->categoryID = -1;
    this->userID = -1;
    this->userName = userName;
    this->categoryName = categoryName;

    this->size = SimpleMessage::getMessageSize() + 4*sizeof(long) + userName.size() + categoryName.size();
}

std::string CategoryManagementMessage::getUserName() const {
    return userName;
}

std::string CategoryManagementMessage::getCategoryName() const {
    return categoryName;
}

long CategoryManagementMessage::getCategoryID() const {
    return categoryID;
}

long CategoryManagementMessage::getUserID() const {
    return userID;
}