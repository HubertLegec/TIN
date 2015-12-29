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

    //TODO
}