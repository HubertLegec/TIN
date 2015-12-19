//
// Created by hubert.legec on 2015-12-15.
//

#include <sstream>
#include "../headers/NetworkEventStrategy.h"
#include "../../../clientEvents/headers/NetworkEvent.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"
#include "../../../networkMessage/headers/RingMessage.h"
#include "../../../clientEvents/headers/ShowInfoEvent.h"

NetworkEventStrategy::NetworkEventStrategy() : BasicEventStrategy(){ }

NetworkEventStrategy::NetworkEventStrategy(Controller* controller) : BasicEventStrategy(controller) { }

void NetworkEventStrategy::serveMessage(BasicEvent event) {
    NetworkEvent& netEvent = dynamic_cast<NetworkEvent&>(event);
    SimpleMessage msg = netEvent.getMessage();

    switch (msg.getMessageType()){
        case MessageType::SERVER_INFO :
            processServerInfo(msg);
            break;
        case MessageType::CATEGORY_LIST :
            processCategoryList(msg);
            break;
        case MessageType::NEIGHBOURS_SET :
            processNeighbourSet(msg);
            break;
        case MessageType::RING_MESSAGE :
            processRingMessage(msg);
            break;
    }
}

void NetworkEventStrategy::processServerInfo(SimpleMessage message) {
    ServerInfoMessage& msg = dynamic_cast<ServerInfoMessage&>(message);
    switch (msg.getInfoType()){
        case ServerInfoMessageType::CATEGORY_CREATED :
            serveServerInfoCategoryCreated(msg);
            break;
        case ServerInfoMessageType::CATEGORY_REMOVED :
            serveServerInfoCategoryRemoved(msg);
            break;
        case ServerInfoMessageType::USER_ADDED :
            serveServerInfoUserAdded(msg);
            break;
        case ServerInfoMessageType::FAIL :
            serveServerInfoFail(msg);
            break;
    }
}

void NetworkEventStrategy::processCategoryList(SimpleMessage message) {
    CategoryListMessage& msg = dynamic_cast<CategoryListMessage&>(message);
    //TODO
}

void NetworkEventStrategy::processNeighbourSet(SimpleMessage message) {
    NeighboursInfoMessage& msg = dynamic_cast<NeighboursInfoMessage&>(message);
    controller->getModel()->updateLeftNeighbour(msg.getCategoryId(),
                                                ConnectionInfo(msg.getLeftNeighbourIP(),msg.getLeftNeighbourPort(), msg.getLeftNeighbourName()));
    controller->getModel()->updateRightNeighbour(msg.getCategoryId(),
                                                 ConnectionInfo(msg.getRightNeighbourIP(), msg.getRightNeighbourPort(), msg.getRightNeighbourName()));
    std::stringstream ss;
    ss << "Neighbours updated: \n";
    ss << "Left neighbour: " << msg.getLeftNeighbourName() << std::endl;
    ss << "Right neighbour: " << msg.getRightNeighbourName() << std::endl;
    controller->getEventsToServe()->push(ShowInfoEvent(ss.str()));
}

void NetworkEventStrategy::processRingMessage(SimpleMessage message) {
    RingMessage& msg = dynamic_cast<RingMessage&>(message);
    //TODO
}



void NetworkEventStrategy::serveServerInfoUserAdded(const ServerInfoMessage &msg) {
    std::string userName = msg.getInfo();
    long userId = msg.getExtraInfo();

    controller->getModel()->setUserName(userName);
    controller->getModel()->setUserId(userId);

    controller->getEventsToServe()->push(ShowInfoEvent("User account created!"));
}

void NetworkEventStrategy::serveServerInfoCategoryCreated(const ServerInfoMessage &msg) {
    std::string categoryName = msg.getInfo();
    long categoryId = msg.getExtraInfo();

    controller->getModel()->addMyCategory(categoryId, categoryName);

    controller->getEventsToServe()->push(ShowInfoEvent("New category created!"));
}

void NetworkEventStrategy::serveServerInfoCategoryRemoved(const ServerInfoMessage &msg) {
    long categoryId = msg.getExtraInfo();

    controller->getModel()->removeCategoryAndData(categoryId);

    controller->getEventsToServe()->push(ShowInfoEvent("One of your category removed!"));
}

void NetworkEventStrategy::serveServerInfoFail(const ServerInfoMessage &msg) {
    std::string info = msg.getInfo();
    controller->getEventsToServe()->push(ShowInfoEvent(info));
}
