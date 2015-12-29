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
        case ChooseMenuOptionEvent::CREATE_CATEGORY :
            if (controller->getModel()->isRegistered()) {
                createCategory();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::DELETE_CATEGORY :
            if (controller->getModel()->isRegistered()) {
                deleteCategory();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::JOIN_CATEGORY :
            if (controller->getModel()->isRegistered()) {
                joinCategory();
            } else {
                createAccount();
            }
            break;
        case ChooseMenuOptionEvent::LEAVE_CATEGORY :
            if (controller->getModel()->isRegistered()) {
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
    shared_ptr<GetMessage> toSend = make_shared<GetMessage>(controller->getModel()->getUserId(),
                                                            GetMessageType::CAT_LIST);
    controller->sendMessage(toSend, controller->getModel()->getServerInfo().getIP(),
                            controller->getModel()->getServerInfo().getPort());
}

void ChooseMenuOptionEventStrategy::createCategory() const {
    controller->getView()->showCreateCategorySubMenu();
}

void ChooseMenuOptionEventStrategy::deleteCategory() const {
    controller->getView()->showDeleteCategorySubMenu(controller->getModel()->getMyCategories());
}

void ChooseMenuOptionEventStrategy::joinCategory() const {
    controller->getView()->showJoinCategorySubMenu();
}

void ChooseMenuOptionEventStrategy::leaveCategory() const {
    controller->getView()->showLeaveCategorySubMenu();
}

void ChooseMenuOptionEventStrategy::createAccount() const {
    controller->getView()->showRegisterNewUserSubMenu();
}

void ChooseMenuOptionEventStrategy::refresh() const {
    controller->getView()->showMainMenu(controller->getModel()->getNotifications());
};