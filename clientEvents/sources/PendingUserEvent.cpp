//
// Created by hubert on 05.01.16.
//

#include <sstream>
#include "../headers/PendingUserEvent.h"

using namespace std;

PendingUserEvent::PendingUserEvent(long categoryID, long userID, PendingUserEvent::Action action)
        : BasicEvent(), categoryID(categoryID), userID(userID), action(action) {

}

PendingUserEvent::Action PendingUserEvent::getAction() const {
    return action;
}

long PendingUserEvent::getUserID() const {
    return userID;
}

long PendingUserEvent::getCategoryID() const {
    return categoryID;
}

string PendingUserEvent::getName() const {
    return "PENDING_USER_EVENT";
}

std::string PendingUserEvent::toString() const {
    stringstream ss;
    ss << "PendingUserEvent[action:" << action << "; ";
    ss << "userID:" << userID << "; ";
    ss << "categoryID:" << categoryID << "]";
    return ss.str();
}
