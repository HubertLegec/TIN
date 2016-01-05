//
// Created by hubert.legec on 2015-12-15.
//
#include "../../../logger/easylogging++.h"
#include "../headers/ChooseMenuOptionEventStrategy.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../../networkMessage/headers/GetMessage.h"

using namespace std;

ChooseMenuOptionEventStrategy::ChooseMenuOptionEventStrategy() : BasicEventStrategy(){ }

ChooseMenuOptionEventStrategy::ChooseMenuOptionEventStrategy(Controller* controller) : BasicEventStrategy(controller) { }

void ChooseMenuOptionEventStrategy::serveEvent(BasicEvent *event) {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::serveEvent:\n" << event->toString();

    ChooseMenuOptionEvent *chooseMenuOptionEvent = dynamic_cast<ChooseMenuOptionEvent *>(event);
    switch (chooseMenuOptionEvent->getOptionChosen()) {
        case ChooseMenuOptionEvent::QUIT :
            controller->exit();
            break;
        case ChooseMenuOptionEvent::SHOW_CATEGORY_LIST :
            showCategoryList();
            break;
        case ChooseMenuOptionEvent::CREATE_USER_ACCOUNT:
            createAccount();
            break;
        case ChooseMenuOptionEvent::REMOVE_USER_ACCOUNT:
            removeAccount();
            break;
        case ChooseMenuOptionEvent::CREATE_CATEGORY :
            createCategory();
            break;
        case ChooseMenuOptionEvent::DELETE_CATEGORY :
            deleteCategory();
            break;
        case ChooseMenuOptionEvent::SIGN_UP_CATEGORY :
            signUpCategory();
            break;
        case ChooseMenuOptionEvent::SIGN_OUT_CATEGORY :
            if (getModel()->isRegistered()) {
                signOutCategory();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::JOIN_CATEGORY :
            if (getModel()->isRegistered()) {
                joinCategory();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::LEAVE_CATEGORY :
            if (getModel()->isRegistered()) {
                leaveCategory();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::SEND_MESSAGE :
            if (getModel()->isRegistered()) {
                sendRingMessage();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::OPEN_INBOX :
            if (getModel()->isRegistered()) {
                openInbox();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::PENDING_USERS :
            if (getModel()->isRegistered()) {
                pendingUsers();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::REFRESH :
            refresh();
            break;
    }
}


void ChooseMenuOptionEventStrategy::showCategoryList() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::showCategoryList\n";
    if (!getModel()->isRegistered()) {
        createAccount();
    } else {
        shared_ptr<GetMessage> toSend = make_shared<GetMessage>(getModel()->getUserId(), GetMessageType::CAT_LIST);
        controller->setState(Controller::CATEGORY_LIST);
        controller->createTimeoutThread();
        controller->sendMessage(toSend, getServerIP(), getServerPort());
    }
}

void ChooseMenuOptionEventStrategy::createCategory() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::createCategory\n";
    if (getModel()->isRegistered()) {
        getView()->showCreateCategorySubMenu();
    } else {
        createAccount();
    }
}

void ChooseMenuOptionEventStrategy::deleteCategory() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::deleteCategory\n";
    if (getModel()->isRegistered()) {
        if (getModel()->getMyCategories().size() == 0) {
            getView()->showInfo("You have no categories to delete!");
            showMainMenu();
        } else {
            getView()->showDeleteCategorySubMenu(getModel()->getMyCategories());
        }
    } else {
        createAccount();
    }
}

void ChooseMenuOptionEventStrategy::joinCategory() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::joinCategory\n";
    if (getModel()->getInactiveCategories().size() == 0) {
        getView()->showInfo("You have no inactive categories");
    } else {
        getView()->showJoinCategorySubMenu(getModel()->getInactiveCategories());
    }
}

void ChooseMenuOptionEventStrategy::leaveCategory() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::leaveCategory\n";
    if (getModel()->getActiveCategories().size() == 0) {
        getView()->showInfo("You have no categories to leave!");
        showMainMenu();
    } else {
        getView()->showLeaveCategorySubMenu(getModel()->getActiveCategories());
    }
}

void ChooseMenuOptionEventStrategy::signUpCategory() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::signUpCategory\n";
    if (getModel()->isRegistered()) {
        controller->setState(Controller::SIGN_UP);
        controller->createTimeoutThread();
        shared_ptr<GetMessage> ptr = make_shared<GetMessage>(getModel()->getUserId(), GetMessageType::CAT_LIST);
        controller->getSendQueue()->push(
                shared_ptr<MessageWrapper>(new MessageWrapper(ptr, getServerIP(), getServerPort())));
    } else {
        createAccount();
    }
}

void ChooseMenuOptionEventStrategy::signOutCategory() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::signOutCategory\n";
    if (getModel()->getJoinedCategories().size() == 0) {
        getView()->showInfo("You have no categories to sign out!");
        showMainMenu();
    } else {
        getView()->showSignOutCategorySubMenu(getModel()->getJoinedCategories());
    }
}

void ChooseMenuOptionEventStrategy::createAccount() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::createAccount\n";
    getView()->showRegisterNewUserSubMenu();
}

void ChooseMenuOptionEventStrategy::removeAccount() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::removeAccount\n";
    if (getModel()->isRegistered()) {
        getView()->showRemoveUserSubMenu();
    } else {
        getView()->showInfo("Your account hasn't been created yet.");
        refresh();
    }
}

void ChooseMenuOptionEventStrategy::refresh() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::refresh\n";
    showMainMenu();
};

void ChooseMenuOptionEventStrategy::sendRingMessage() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::sendRingMessage\n";
    if (getModel()->getMyCategories().size() == 0) {
        getView()->showInfo("You have no categories to send message!");
        showMainMenu();
    } else {
        getView()->sendMessageInCategorySubMenu(getModel()->getMyCategories());
    }
}

void ChooseMenuOptionEventStrategy::openInbox() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::openInbox\n";
    if (getModel()->getInboxMessages().size() == 0) {
        getView()->showInfo("Your inbox is empty. Check it later.");
        showMainMenu();
    } else {
        vector<pair<string, string>> messages;
        for (auto msg : getModel()->getInboxMessages()) {
            messages.push_back(
                    pair<string, string>(getModel()->getCategoryName(msg.getCategoryId()), msg.getMsgText()));
        }
        getView()->showReadIncomingMessagesSubMenu(messages);
    }
}

void ChooseMenuOptionEventStrategy::pendingUsers() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::pendingUsers\n";
    if (getModel()->getPendingUsers().size() == 0) {
        getView()->showInfo("Nobody wants to join to your category.");
        showMainMenu();
    } else {
        getView()->showPendingUsersSubMenu(getModel()->getPendingUsers());
    }
}