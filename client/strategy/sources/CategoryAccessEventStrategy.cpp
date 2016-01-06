//
// Created by hubert on 29.12.15.
//

#include "../headers/CategoryAccessEventStrategy.h"
#include "../../../clientEvents/headers/CategoryAccessEvent.h"
#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../logger/easylogging++.h"

using namespace std;

CategoryAccessEventStrategy::CategoryAccessEventStrategy() : BasicEventStrategy() {

}

CategoryAccessEventStrategy::CategoryAccessEventStrategy(Controller *controller) : BasicEventStrategy(controller) {

}

void CategoryAccessEventStrategy::serveEvent(BasicEvent *event) {
    LOG(INFO) << "CategoryAccessEventStrategy::serveEvent:\n" << event->toString();

    CategoryAccessEvent *categoryAccessEvent = dynamic_cast<CategoryAccessEvent *>(event);
    controller->createTimeoutThread();
    switch (categoryAccessEvent->getType()) {
        case CategoryAccessEvent::CREATE_CATEGORY :
            createCategory(categoryAccessEvent->getCategoryName());
            break;
        case CategoryAccessEvent::DELETE_CATEGORY :
            deleteCategory(categoryAccessEvent->getCategoryID());
            break;
        case CategoryAccessEvent::SIGN_UP_CATEGORY :
            signUpCategory(categoryAccessEvent->getCategoryID());
            break;
        case CategoryAccessEvent::SIGN_OUT_CATEGORY :
            signOutCategory(categoryAccessEvent->getCategoryID());
            break;
        case CategoryAccessEvent::JOIN_CATEGORY :
            joinCategory(categoryAccessEvent->getCategoryID());
            break;
        case CategoryAccessEvent::LEAVE_CATEGORY :
            leaveCategory(categoryAccessEvent->getCategoryID());
            break;
    }
}

void CategoryAccessEventStrategy::createCategory(const std::string &name) const {
    LOG(INFO) << "CategoryAccessEventStrategy::createCategory:\n" << "categoryName: " << name;

    auto msg = shared_ptr<CategoryManagementMessage>(new CategoryManagementMessage(getModel()->getUserId(),
                                                                                   MessageType::CREATE_CATEGORY,
                                                                                   name));
    controller->sendMessage(msg, getServerIP(), getServerPort());
}

void CategoryAccessEventStrategy::deleteCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::deleteCategory:\n" << "categoryID: " << id;
    auto msg = shared_ptr<CategoryManagementMessage>(new CategoryManagementMessage(getModel()->getUserId(),
                                                                                   MessageType::DESTROY_CATEGORY,
                                                                                   id));
    controller->sendMessage(msg, getServerIP(), getServerPort());
}

void CategoryAccessEventStrategy::signUpCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::signUpCategory:\n" << "categoryID: " << id;
    auto msg = shared_ptr<CategoryManagementMessage>(new CategoryManagementMessage(getModel()->getUserId(),
                                                                                   MessageType::JOIN_CATEGORY, id));
    controller->sendMessage(msg, getServerIP(), getServerPort());
}

void CategoryAccessEventStrategy::signOutCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::signOutCategory:\n" << "categoryID: " << id;
    auto msg = shared_ptr<CategoryManagementMessage>(new CategoryManagementMessage(getModel()->getUserId(),
                                                                                   MessageType::LEFT_CATEGORY, id));
    controller->sendMessage(msg, getServerIP(), getServerPort());
}

void CategoryAccessEventStrategy::joinCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::joinCategory:\n" << "categoryID: " << id;
    auto msg = shared_ptr<CategoryManagementMessage>(new CategoryManagementMessage(getModel()->getUserId(),
                                                                                   MessageType::ACTIVATE_CATEGORY,
                                                                                   id));
    controller->sendMessage(msg, getServerIP(), getServerPort());
}

void CategoryAccessEventStrategy::leaveCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::leaveCategory:\n" << "categoryID: " << id;
    auto msg = shared_ptr<CategoryManagementMessage>(new CategoryManagementMessage(getModel()->getUserId(),
                                                                                   MessageType::DEACTIVATE_CATEGORY,
                                                                                   id));
    controller->sendMessage(msg, getServerIP(), getServerPort());
}