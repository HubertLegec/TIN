//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/CategoryManagementMessage.h"

using namespace std;

CategoryManagementMessage::CategoryManagementMessage() : SimpleMessage() {
    this->size = SimpleMessage::getMessageSize() + 2 * sizeof(long);
}

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, long categoryID)
                                                    : SimpleMessage(type, senderID){
    this->categoryID = categoryID;

    this->size = SimpleMessage::getMessageSize() + 2 * sizeof(long);
}

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, const string &categoryName)
        : SimpleMessage(type, senderID) {
    this->categoryID = UNDEFINED_ID;
    this->categoryName = categoryName;

    this->size = SimpleMessage::getMessageSize() + 2 * sizeof(long) + categoryName.size();
}

const string &CategoryManagementMessage::getCategoryName() const {
    return categoryName;
}

long CategoryManagementMessage::getCategoryID() const {
    return categoryID;
}

long CategoryManagementMessage::getExtraInfo() const {
    return extraInfo;
}

string CategoryManagementMessage::toString() const {
    stringstream ss;
    ss << "CategoryManagementMessage[type:" << type << "; ";
    ss << "size:" << size << "; ";
    ss << "senderID:" << senderID << ";\n";
    ss << "categoryName:" << categoryName << "; ";
    ss << "categoryID:" << categoryID << "]";
    return ss.str();
}