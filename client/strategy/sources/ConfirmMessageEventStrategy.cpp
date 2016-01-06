//
// Created by hubert.legec on 2015-12-15.
//

#include "../headers/ConfirmMessageEventStrategy.h"
#include "../../../clientEvents/headers/ConfirmMessageEvent.h"

using namespace std;

ConfirmMessageEventStrategy::ConfirmMessageEventStrategy() : BasicEventStrategy() { }

ConfirmMessageEventStrategy::ConfirmMessageEventStrategy(Controller* controller) : BasicEventStrategy(controller) { }

void ConfirmMessageEventStrategy::serveEvent(BasicEvent *event) {
    ConfirmMessageEvent *confirmMessageEvent = dynamic_cast<ConfirmMessageEvent *>(event);
    long msgIndex = confirmMessageEvent->getMessageIndex();
    RingMessage m = getModel()->getInboxMessages()[msgIndex];
    getModel()->markMessageAsRead(msgIndex);

    m.addConfirmation(getModel()->getUserName());
    ConnectionInfo neighbour = getModel()->getLeftNeighbour(m.getCategoryId());
    auto toSend = shared_ptr<RingMessage>(new RingMessage(m));
    controller->sendMessage(toSend, neighbour.getIP(), neighbour.getPort());
}