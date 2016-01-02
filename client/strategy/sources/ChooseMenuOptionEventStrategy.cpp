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
            if (getModel()->isRegistered()) {
                showCategoryList();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::CREATE_USER_ACCOUNT:
            createAccount();
            break;
        case ChooseMenuOptionEvent::CREATE_CATEGORY :
            if (getModel()->isRegistered()) {
                createCategory();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::DELETE_CATEGORY :
            if (getModel()->isRegistered()) {
                deleteCategory();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::SIGN_UP_CATEGORY :
            if (getModel()->isRegistered()) {
                signUpCategory();
            } else {
                createAccount();
            }
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
        case ChooseMenuOptionEvent::REFRESH :
            refresh();
            break;
    }
}


void ChooseMenuOptionEventStrategy::showCategoryList() const {
    LOG(INFO) << "ChooseMenuOptionEventStrategy::showCategoryList:\n" << "userID: " << getModel()->getUserId();
    shared_ptr<GetMessage> toSend = make_shared<GetMessage>(getModel()->getUserId(), GetMessageType::CAT_LIST);
    controller->setState(Controller::CATEGORY_LIST);
    controller->sendMessage(toSend, getServerIP(), getServerPort());
}

void ChooseMenuOptionEventStrategy::createCategory() const {
    getView()->showCreateCategorySubMenu();
}

void ChooseMenuOptionEventStrategy::deleteCategory() const {
    getView()->showDeleteCategorySubMenu(getModel()->getMyCategories());
}

void ChooseMenuOptionEventStrategy::joinCategory() const {
    getView()->showJoinCategorySubMenu(getModel()->getInactiveCategories());
}

void ChooseMenuOptionEventStrategy::leaveCategory() const {
    getView()->showLeaveCategorySubMenu(getModel()->getActiveCategories());
}

void ChooseMenuOptionEventStrategy::signUpCategory() const {
    controller->setState(Controller::SIGN_UP);
    shared_ptr<GetMessage> ptr = make_shared<GetMessage>(getModel()->getUserId(), GetMessageType::CAT_LIST);
    controller->getSendQueue()->push(
            shared_ptr<MessageWrapper>(new MessageWrapper(ptr, getServerIP(), getServerPort())));
}

void ChooseMenuOptionEventStrategy::signOutCategory() const {
    getView()->showSignOutCategorySubMenu(getModel()->getJoinedCategories());
}

void ChooseMenuOptionEventStrategy::createAccount() const {
    getView()->showRegisterNewUserSubMenu();
}

void ChooseMenuOptionEventStrategy::refresh() const {
    getView()->showMainMenu(getModel()->getNotifications());
};