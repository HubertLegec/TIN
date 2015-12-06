//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/CategoryManagementMessage.h"

CategoryManagementMessage::CategoryManagementMessage(long senderID, MessageType type, const std::string& categoryName, const std::string& userName)
                                                    : SimpleMessage(type, senderID) {
    this->userNameSize = userName.size();
    this->userName = new char[userNameSize+1];
    strcpy(this->userName, userName.c_str());
    this->categoryNameSize = categoryName.size();
    this->categoryName = new char[categoryNameSize+1];
    strcpy(this->categoryName, categoryName.c_str());

    this->size = SimpleMessage::getMessageSize() + 2*sizeof(int) + (userNameSize + categoryNameSize + 2);
}


CategoryManagementMessage::CategoryManagementMessage(char *data) : SimpleMessage(data) {
    //TODO
}

CategoryManagementMessage::~CategoryManagementMessage() {
    delete[] userName;
    delete[] categoryName;
}

std::string CategoryManagementMessage::getUserName() const {
    return std::string(userName);
}

std::string CategoryManagementMessage::getCategoryName() const {
    return std::string(categoryName);
}