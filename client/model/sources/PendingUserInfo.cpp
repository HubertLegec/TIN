//
// Created by hubert on 05.01.16.
//

#include "../headers/PendingUserInfo.h"

using namespace std;

PendingUserInfo::PendingUserInfo() { }

PendingUserInfo::PendingUserInfo(long categoryID, long userID, const string &userName, const string &categoryName)
        : categoryID(categoryID), userID(userID), userName(userName), categoryName(categoryName) { }

void PendingUserInfo::setCategoryID() {
    this->categoryID = categoryID;
}

long PendingUserInfo::getCategoryID() const {
    return categoryID;
}

void PendingUserInfo::setUserID() {
    this->userID = userID;
}

long PendingUserInfo::getUserID() const {
    return userID;
}

void PendingUserInfo::setUserName(const std::string &userName) {
    this->userName = userName;
}

const string &PendingUserInfo::getUserName() const {
    return userName;
}

void PendingUserInfo::setCategoryName(const std::string &categoryName) {
    this->categoryName = categoryName;
}

const std::string &PendingUserInfo::getCategoryName() const {
    return categoryName;
}