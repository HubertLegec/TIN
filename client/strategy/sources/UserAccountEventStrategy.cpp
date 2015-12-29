//
// Created by hubert on 29.12.15.
//

#include "../headers/UserAccountEventStrategy.h"
#include "../../../clientEvents/headers/UserAccountEvent.h"
#include "../../../logger/easylogging++.h"

UserAccountEventStrategy::UserAccountEventStrategy() : BasicEventStrategy() {

}

UserAccountEventStrategy::UserAccountEventStrategy(Controller *controller) : BasicEventStrategy(controller) {

}

void UserAccountEventStrategy::serveEvent(BasicEvent *event) {
    LOG(INFO) << "CategoryAccessEventStrategy::serveEvent:\n" << event->toString();

    UserAccountEvent *categoryAccessEvent = dynamic_cast<UserAccountEvent *>(event);
}