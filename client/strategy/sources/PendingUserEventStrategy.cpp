//
// Created by hubert on 05.01.16.
//

#include "../headers/PendingUserEventStrategy.h"
#include "../../../logger/easylogging++.h"
#include "../../../clientEvents/headers/PendingUserEvent.h"
#include "../../../networkMessage/headers/CategoryManagementMessage.h"

using namespace std;

PendingUserEventStrategy::PendingUserEventStrategy() : BasicEventStrategy() { }

PendingUserEventStrategy::PendingUserEventStrategy(Controller *controller) : BasicEventStrategy(controller) { }

void PendingUserEventStrategy::serveEvent(BasicEvent *event) {
    LOG(INFO) << "PendingUserEventStrategy::serveEvent:\n" << event->toString();

    PendingUserEvent *pendingUserEvent = dynamic_cast<PendingUserEvent *>(event);

    switch (pendingUserEvent->getAction()) {
        case PendingUserEvent::ACCEPT:
            acceptUser(pendingUserEvent->getCategoryID(), pendingUserEvent->getUserID());
            break;
        case PendingUserEvent::REJECT:
            rejectUser(pendingUserEvent->getCategoryID(), pendingUserEvent->getUserID());
            break;
    }

}

void PendingUserEventStrategy::acceptUser(long categoryID, long userID) {
    LOG(INFO) << "PendingUserEventStrategy::acceptUser:\ncategoryID:" << categoryID << " ; userID:" << userID;
    auto msg = shared_ptr<CategoryManagementMessage>(new CategoryManagementMessage(getModel()->getUserId(),
                                                                                   MessageType::NEW_MEMBER_CONFIRM,
                                                                                   categoryID, userID));
    controller->sendMessage(msg, getServerIP(), getServerPort());
    getModel()->removePendingUser(categoryID, userID);
}

void PendingUserEventStrategy::rejectUser(long categoryID, long userID) {
    LOG(INFO) << "PendingUserEventStrategy::rejectUser:\ncategoryID:" << categoryID << " ; userID:" << userID;
    auto msg = shared_ptr<CategoryManagementMessage>(new CategoryManagementMessage(getModel()->getUserId(),
                                                                                   MessageType::NEW_MEMBER_REJECT,
                                                                                   categoryID, userID));
    controller->sendMessage(msg, getServerIP(), getServerPort());
    getModel()->removePendingUser(categoryID, userID);
}