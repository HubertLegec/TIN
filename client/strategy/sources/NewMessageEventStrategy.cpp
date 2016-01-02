//
// Created by hubert on 02.01.16.
//

#include "../headers/NewMessageEventStrategy.h"
#include "../../../clientEvents/headers/NewMessageEvent.h"
#include "../../../logger/easylogging++.h"

using namespace std;

NewMessageEventStrategy::NewMessageEventStrategy() : BasicEventStrategy() { }

NewMessageEventStrategy::NewMessageEventStrategy(Controller *controller) : BasicEventStrategy(controller) { }

void NewMessageEventStrategy::serveEvent(BasicEvent *event) {
    LOG(INFO) << "NewMessageEventStrategy::serveEvent:\n" << event->toString();

    NewMessageEvent *newMessageEvent = dynamic_cast<NewMessageEvent *>(event);
    shared_ptr<RingMessage> ringMsg = make_shared<RingMessage>(getModel()->getUserId(),
                                                               newMessageEvent->getCategoryID(),
                                                               newMessageEvent->getMessage());
    ConnectionInfo neighbour = getModel()->getLeftNeighbour(newMessageEvent->getCategoryID());
    shared_ptr<MessageWrapper> msg = shared_ptr<MessageWrapper>(
            new MessageWrapper(ringMsg, neighbour.getIP(), neighbour.getPort()));
    sendMessage(msg);
    showMainMenu();
}