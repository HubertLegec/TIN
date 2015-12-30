//
// Created by hubert on 29.12.15.
//

#include "../headers/CategoryAccessEventStrategy.h"
#include "../../../clientEvents/headers/CategoryAccessEvent.h"
#include "../../../logger/easylogging++.h"

using namespace std;

CategoryAccessEventStrategy::CategoryAccessEventStrategy() : BasicEventStrategy() {

}

CategoryAccessEventStrategy::CategoryAccessEventStrategy(Controller *controller) : BasicEventStrategy(controller) {

}

void CategoryAccessEventStrategy::serveEvent(BasicEvent *event) {
    LOG(INFO) << "CategoryAccessEventStrategy::serveEvent:\n" << event->toString();

    CategoryAccessEvent *categoryAccessEvent = dynamic_cast<CategoryAccessEvent *>(event);
    switch (categoryAccessEvent->getType()) {
        case CategoryAccessEvent::CREATE_CATEGORY :
            createCategory(categoryAccessEvent->getName());
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
    //TODO
}

void CategoryAccessEventStrategy::deleteCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::deleteCategory:\n" << "categoryID: " << id;
    //TODO
}

void CategoryAccessEventStrategy::signUpCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::signUpCategory:\n" << "categoryID: " << id;
    //TODO
}

void CategoryAccessEventStrategy::signOutCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::signOutCategory:\n" << "categoryID: " << id;
    //TODO
}

void CategoryAccessEventStrategy::joinCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::joinCategory:\n" << "categoryID: " << id;
    //TODO
}

void CategoryAccessEventStrategy::leaveCategory(long id) const {
    LOG(INFO) << "CategoryAccessEventStrategy::leaveCategory:\n" << "categoryID: " << id;
    //TODO
}