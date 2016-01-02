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

    switch (userAccountEvent->getType()) {
        case UserAccountEvent::CREATE :
            createAccount(userAccountEvent->getUserName());
            showMainMenu();
            break;
        case UserAccountEvent::DELETE :
            deleteAccount();
            showMainMenu();
            break;
    }
}

void UserAccountEventStrategy::createAccount(const std::string &name) const {
    LOG(INFO) << "CUserAccountEventStrategy::createAccount:\n" << "userName: " << name;
    shared_ptr<UserManagementMessage> msg = shared_ptr<UserManagementMessage>(
            new UserManagementMessage(MessageType::CREATE_USER_ACCOUNT, getMyIP(), getMyPort(), name));
    controller->getSendQueue()->push(
            shared_ptr<MessageWrapper>(new MessageWrapper(msg, getServerIP(), getServerPort())));
}

void UserAccountEventStrategy::deleteAccount() const {
    LOG(INFO) << "CUserAccountEventStrategy::DeleteAccount";
    long id = getModel()->getUserId();
    shared_ptr<UserManagementMessage> msg = shared_ptr<UserManagementMessage>(
            new UserManagementMessage(id, MessageType::DELETE_USER_ACCOUNT));
    controller->getSendQueue()->push(
            shared_ptr<MessageWrapper>(new MessageWrapper(msg, getServerIP(), getServerPort())));
}