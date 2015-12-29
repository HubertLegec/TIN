//
// Created by hubert on 29.12.15.
//

#include <sstream>
#include "../headers/UserAccountEvent.h"

using namespace std;

UserAccountEvent::UserAccountEvent(const std::string &userName) : userName(userName) {

}

const std::string &UserAccountEvent::getUserName() const {
    return userName;
}

std::string UserAccountEvent::toString() const {
    stringstream ss;
    ss << "UserAccountEvent[userName:" << userName << "]";
    return ss.str();
}

std::string UserAccountEvent::getName() const {
    return "USER_ACCOUNT_EVENT";
}