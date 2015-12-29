//
// Created by hubert.legec on 2015-12-15.
//
#include "../../../logger/easylogging++.h"
#include "../headers/ChooseMenuOptionEventStrategy.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../../networkMessage/headers/GetMessage.h"

ChooseMenuOptionEventStrategy::ChooseMenuOptionEventStrategy() : BasicEventStrategy(){ }

ChooseMenuOptionEventStrategy::ChooseMenuOptionEventStrategy(Controller* controller) : BasicEventStrategy(controller) { }

void ChooseMenuOptionEventStrategy::serveEvent(BasicEvent *event) {
    std::cout << "ChooseMenuOptionEventStrategy: serveEvent\n";
    ChooseMenuOptionEvent *chooseMenuOptionEvent = dynamic_cast<ChooseMenuOptionEvent *>(event);
    switch (chooseMenuOptionEvent->getOptionChosen()) {
        case ChooseMenuOptionEvent::QUIT :
            if (controller->getView()->getUserConfirmation()) {
                controller->exit();
            }
            break;
        case ChooseMenuOptionEvent::SHOW_CATEGORY_LIST :
            showCategoryList();
            break;
        case ChooseMenuOptionEvent::CREATE_CATEGORY :
            createCategory();
            break;
        case ChooseMenuOptionEvent::DELETE_CATEGORY :

            break;
    }
}


void ChooseMenuOptionEventStrategy::showCategoryList() const {
    std::shared_ptr<GetMessage> toSend = std::make_shared<GetMessage>(controller->getModel()->getUserId(),
                                                                      GetMessageType::CAT_LIST);
    controller->sendMessage(toSend, controller->getModel()->getServerInfo().getIP(),
                            controller->getModel()->getServerInfo().getPort());
}

void ChooseMenuOptionEventStrategy::createCategory() const {
    controller->getView()->showCreateCategorySubMenu();
}