//
// Created by hubert.legec on 2015-12-15.
//

#include <sstream>
#include "../../model/headers/Model.h"
#include "../../view/headers/View.h"
#include "../headers/NetworkEventStrategy.h"
#include "../../../clientEvents/headers/NetworkEvent.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"

NetworkEventStrategy::NetworkEventStrategy() : BasicEventStrategy(){ }

NetworkEventStrategy::NetworkEventStrategy(Controller* controller) : BasicEventStrategy(controller) { }

void NetworkEventStrategy::serveEvent(BasicEvent *event) {
    NetworkEvent *netEvent = dynamic_cast<NetworkEvent *>(event);
    std::shared_ptr<SimpleMessage> msg = netEvent->getMessage();

    switch (msg->getMessageType()) {
        case MessageType::SERVER_INFO :
            processServerInfo(*msg);
            break;
        case MessageType::CATEGORY_LIST :
            processCategoryList(*msg);
            break;
        case MessageType::NEIGHBOURS_SET :
            processNeighbourSet(*msg);
            break;
        case MessageType::RING_MESSAGE :
            processRingMessage(*msg);
            break;
    }
}

void NetworkEventStrategy::processServerInfo(SimpleMessage &message) const {
    ServerInfoMessage& msg = dynamic_cast<ServerInfoMessage&>(message);
    switch (msg.getInfoType()){
        case ServerInfoMessageType::CATEGORY_CREATED :
            controller->getModel()->addMyCategory(msg.getExtraInfo(), msg.getInfo());
            controller->getModel()->addNotification("New category created!");
            break;

        case ServerInfoMessageType::CATEGORY_REMOVED :
            controller->getModel()->removeCategoryAndData(msg.getExtraInfo());
            controller->getModel()->addNotification("One of your category removed!");
            break;

        case ServerInfoMessageType::USER_ADDED :
            controller->getModel()->setUserName(msg.getInfo());
            controller->getModel()->setUserId(msg.getExtraInfo());
            controller->getModel()->addNotification("User account created!");
            break;

        case ServerInfoMessageType::CATEGORY_JOINED :
            controller->getModel()->addJoinedCategory(msg.getExtraInfo(), msg.getInfo());
            controller->getModel()->addNotification("You have successfully joined the category!");
            break;

        case ServerInfoMessageType::CATEGORY_LEFT :
            controller->getModel()->removeCategoryAndData(msg.getExtraInfo());
            controller->getModel()->addNotification("You have successfully left the category!");
            break;

        case ServerInfoMessageType::CATEGORY_ACTIVATED :
            controller->getModel()->setCategoryActive(msg.getExtraInfo(), true);
            controller->getModel()->addNotification("Category is active again!");
            break;

        case ServerInfoMessageType::CATEGORY_DEACTIVATED :
            controller->getModel()->setCategoryActive(msg.getExtraInfo(), false);
            controller->getModel()->addNotification("Category deactivated!");
            break;

        case ServerInfoMessageType::FAIL :
            controller->getModel()->addNotification(msg.getInfo());
            break;
    }
}

void NetworkEventStrategy::processCategoryList(SimpleMessage &message) const {
    CategoryListMessage& msg = dynamic_cast<CategoryListMessage&>(message);
    controller->getView()->showCategoryList(msg.getCategories());
}

void NetworkEventStrategy::processNeighbourSet(SimpleMessage &message) const {
    NeighboursInfoMessage& msg = dynamic_cast<NeighboursInfoMessage&>(message);
    controller->getModel()->updateLeftNeighbour(msg.getCategoryId(),
                                                ConnectionInfo(msg.getLeftNeighbourIP(),msg.getLeftNeighbourPort(), msg.getLeftNeighbourName()));
    controller->getModel()->updateRightNeighbour(msg.getCategoryId(),
                                                 ConnectionInfo(msg.getRightNeighbourIP(), msg.getRightNeighbourPort(), msg.getRightNeighbourName()));
    std::stringstream ss;
    ss << "Neighbours updated: \n";
    ss << "Left neighbour: " << msg.getLeftNeighbourName() << std::endl;
    ss << "Right neighbour: " << msg.getRightNeighbourName() << std::endl;
    controller->getModel()->addNotification(ss.str());
}

void NetworkEventStrategy::processRingMessage(SimpleMessage &message) const {
    RingMessage& msg = dynamic_cast<RingMessage&>(message);
    controller->getModel()->addMessageToInbox(msg);
    controller->getModel()->addNotification("You have a new message!\nCheck your inbox.");
}
