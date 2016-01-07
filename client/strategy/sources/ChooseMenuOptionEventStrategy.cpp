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
            signOutCategory();
            break;
        case ChooseMenuOptionEvent::JOIN_CATEGORY :
            joinCategory();
            break;
        case ChooseMenuOptionEvent::LEAVE_CATEGORY :
            leaveCategory();
            break;
        case ChooseMenuOptionEvent::SEND_MESSAGE :
            sendRingMessage();
            break;
        case ChooseMenuOptionEvent::OPEN_INBOX :
            openInbox();
            break;
        case ChooseMenuOptionEvent::PENDING_USERS :
            pendingUsers();
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
        auto toSend = shared_ptr<GetMessage>(new GetMessage(getModel()->getUserId(), GetMessageType::CAT_LIST));
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
    if (getModel()->isRegistered()) {
        if (getModel()->getInactiveCategories().size() == 0) {
            getView()->showInfo("You have no inactive categories");
            showMainMenu();
        } else {
            getView()->showJoinCategorySubMenu(getModel()->getInactiveCategories());
        }
    } else {
        createAccount();
    }
}

void ChooseMenuOptionEventStrategy::leaveCategory() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::leaveCategory\n";
    if (getModel()->isRegistered()) {
        if (getModel()->getActiveCategories().size() == 0) {
            getView()->showInfo("You have no categories to leave!");
            showMainMenu();
        } else {
            getView()->showLeaveCategorySubMenu(getModel()->getActiveCategories());
        }
    }
}

void ChooseMenuOptionEventStrategy::signUpCategory() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::signUpCategory\n";
    if (getModel()->isRegistered()) {
        controller->setState(Controller::SIGN_UP);
        controller->createTimeoutThread();
        auto ptr = shared_ptr<GetMessage>(new GetMessage(getModel()->getUserId(), GetMessageType::CAT_LIST));
        controller->sendMessage(ptr, getServerIP(), getServerPort());
    } else {
        createAccount();
    }
}

void ChooseMenuOptionEventStrategy::signOutCategory() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::signOutCategory\n";
    if (getModel()->isRegistered()) {
        if (getModel()->getJoinedCategories().size() == 0) {
            getView()->showInfo("You have no categories to sign out!");
            showMainMenu();
        } else if (getModel()->getInboxMessages().size() > 0) {
            getView()->showInfo("Your inbox is not empty! Read all the messages and then come back here.");
            showMainMenu();
        } else {
            getView()->showSignOutCategorySubMenu(getModel()->getJoinedCategories());
        }
    } else {
        createAccount();
    }
}

void ChooseMenuOptionEventStrategy::createAccount() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::createAccount\n";
    if (!getModel()->isRegistered()) {
        getView()->showRegisterNewUserSubMenu();
    } else {
        getView()->showInfo("Your account has been created.");
        showMainMenu();
    }
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
    if (getModel()->isRegistered()) {
        if (getModel()->getMyCategories().size() == 0) {
            getView()->showInfo("You have no categories to send message!");
            showMainMenu();
        } else {
            vector<string> inactiveAndEmpty = getModel()->getMyEmptyAndNonActiveCategories();
            if (inactiveAndEmpty.size() > 0) {
                stringstream ss;
                for (auto c : inactiveAndEmpty) {
                    ss << c << endl;
                }
                getView()->showInfo(ss.str());
            }
            map<long, string> categories = getModel()->getMyNonEmptyCategories();
            if (categories.size() > 0) {
                getView()->sendMessageInCategorySubMenu(categories);
            } else {
                getView()->showMainMenu(getModel()->getNotifications());
            }
        }
    } else {
        createAccount();
    }
}

void ChooseMenuOptionEventStrategy::openInbox() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::openInbox\n";
    if (getModel()->isRegistered()) {
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
    } else {
        createAccount();
    }
}

void ChooseMenuOptionEventStrategy::pendingUsers() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::pendingUsers\n";
    if (getModel()->isRegistered()) {
        if (getModel()->getPendingUsers().size() == 0) {
            getView()->showInfo("Nobody wants to join to your category.");
            showMainMenu();
        } else {
            getView()->showPendingUsersSubMenu(getModel()->getPendingUsers());
        }
    } else {
        createAccount();
    }
}