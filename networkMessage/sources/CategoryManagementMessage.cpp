//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/CategoryManagementMessage.h"

CategoryManagementMessage::CategoryManagementMessage() { }

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, long categoryID)
                                                    : SimpleMessage(type, senderID){
    this->categoryID = categoryID;

    this->size = SimpleMessage::getMessageSize() + 2 * sizeof(long);
}

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, const std::string &categoryName)
        : SimpleMessage(type, senderID) {
    this->categoryID = UNDEFINED_ID;
    this->categoryName = categoryName;

    this->size = SimpleMessage::getMessageSize() + 2 * sizeof(long) + categoryName.size();
}

std::string CategoryManagementMessage::getCategoryName() const {
    return categoryName;
}

long CategoryManagementMessage::getCategoryID() const {
    return categoryID;
}

std::string CategoryManagementMessage::toString() {
    std::stringstream ss;
    ss << "CategoryListMessage[type:" << type << "; ";
    ss << "size:" << size << "; ";
    ss << "senderID:" << senderID << "; ";
    ss << "categoryName:" << categoryName << "; ";
    ss << "categoryID:" << categoryID << "]";
    return ss.str();
}