//
// Created by hubert.legec on 2015-12-15.
//

#include "../headers/ConfirmMessageEventStrategy.h"
#include "../../../clientEvents/headers/ConfirmMessageEvent.h"

ConfirmMessageEventStrategy::ConfirmMessageEventStrategy() : BasicEventStrategy() { }

ConfirmMessageEventStrategy::ConfirmMessageEventStrategy(Controller* controller) : BasicEventStrategy(controller) { }

void ConfirmMessageEventStrategy::serveEvent(BasicEvent event) {
    ConfirmMessageEvent& confirmMessageEvent = dynamic_cast<ConfirmMessageEvent&>(event);
    //TODO
}