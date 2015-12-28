//
// Created by hubert.legec on 2015-12-15.
//

#include "../headers/ConfirmMessageEventStrategy.h"
#include "../../../clientEvents/headers/ConfirmMessageEvent.h"

ConfirmMessageEventStrategy::ConfirmMessageEventStrategy() : BasicEventStrategy() { }

ConfirmMessageEventStrategy::ConfirmMessageEventStrategy(Controller* controller) : BasicEventStrategy(controller) { }

void ConfirmMessageEventStrategy::serveEvent(BasicEvent *event) const {
    ConfirmMessageEvent *confirmMessageEvent = dynamic_cast<ConfirmMessageEvent *>(event);
    long msgIndex = confirmMessageEvent->getMessageIndex();
    RingMessage m = controller->getModel()->getInboxMessages()[msgIndex];
    controller->getModel()->markMessageAsRead(msgIndex);
    ConnectionInfo sendInfo = controller->getModel()->getLeftNeighbour(m.getCategoryId());

    //TODO
    //insert message to output queue
}