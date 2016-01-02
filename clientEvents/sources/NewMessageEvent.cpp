//
// Created by hubert on 02.01.16.
//

#include <sstream>
#include "../headers/NewMessageEvent.h"

using namespace std;

NewMessageEvent::NewMessageEvent(long categoryID, const std::string &message)
        : BasicEvent(), categoryID(categoryID), message(message) {

}

long NewMessageEvent::getCategoryID() const {
    return categoryID;
}

const std::string &NewMessageEvent::getMessage() const {
    return message;
}

std::string NewMessageEvent::toString() const {
    stringstream ss;
    ss << "NewMessageEvent[categoryID:" << categoryID << "; ";
    ss << "message:" << message << "]";
    return ss.str();
}

std::string NewMessageEvent::getName() const {
    return "NEW_MESSAGE_EVENT";
}