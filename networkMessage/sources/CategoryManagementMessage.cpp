//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/CategoryManagementMessage.h"

CategoryManagementMessage::CategoryManagementMessage() { }

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, long categoryID, long userID)
                                                    : SimpleMessage(type, senderID){
    this->categoryID = categoryID;
    this->userID = userID;

    this->size = SimpleMessage::getMessageSize() + 3 * sizeof(long);
}

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, const std::string &categoryName,
                                                     long userID) : SimpleMessage(type, senderID) {
    this->categoryID = UNDEFINED_ID;
    this->userID = userID;
    this->categoryName = categoryName;

    this->size = SimpleMessage::getMessageSize() + 3 * sizeof(long) + categoryName.size();
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

std::string CategoryManagementMessage::toString() {
    //TODO
    return "CategoryListMessage";
}