//
// Created by hubert on 29.12.15.
//

#include "../headers/UserAccountEventStrategy.h"
#include "../../../clientEvents/headers/UserAccountEvent.h"
#include "../../../networkMessage/headers/UserManagementMessage.h"
#include "../../../logger/easylogging++.h"

using namespace std;

UserAccountEventStrategy::UserAccountEventStrategy() : BasicEventStrategy() {

}

UserAccountEventStrategy::UserAccountEventStrategy(Controller *controller) : BasicEventStrategy(controller) {

}

void UserAccountEventStrategy::serveEvent(BasicEvent *event) {
    LOG(INFO) << "UserAccountEventStrategy::serveEvent:\n" << event->toString();

    UserAccountEvent *userAccountEvent = dynamic_cast<UserAccountEvent *>(event);
    controller->createTimeoutThread();

    switch (userAccountEvent->getType()) {
        case UserAccountEvent::CREATE :
            createAccount(userAccountEvent->getUserName());
            break;
        case UserAccountEvent::DELETE :
            deleteAccount();
            break;
    }
}

void UserAccountEventStrategy::createAccount(const std::string &name) const {
    LOG(INFO) << "UserAccountEventStrategy::createAccount:\n" << "userName: " << name;
    getModel()->setUserName(name);
    auto msg = shared_ptr<UserManagementMessage>(
            new UserManagementMessage(MessageType::CREATE_USER_ACCOUNT, getMyIP(), getMyPort(), name));
    controller->getSendQueue()->push(
            shared_ptr<MessageWrapper>(new MessageWrapper(msg, getServerIP(), getServerPort())));
}

void UserAccountEventStrategy::deleteAccount() const {
    LOG(INFO) << "UserAccountEventStrategy::DeleteAccount";
    long id = getModel()->getUserId();
    auto msg = shared_ptr<UserManagementMessage>(
            new UserManagementMessage(id, MessageType::DELETE_USER_ACCOUNT));
    controller->getSendQueue()->push(
            shared_ptr<MessageWrapper>(new MessageWrapper(msg, getServerIP(), getServerPort())));
}