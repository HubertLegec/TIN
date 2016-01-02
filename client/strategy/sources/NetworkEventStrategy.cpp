//
// Created by hubert.legec on 2015-12-15.
//

#include <sstream>
#include "../../../logger/easylogging++.h"
#include "../../model/headers/Model.h"
#include "../../view/headers/View.h"
#include "../headers/NetworkEventStrategy.h"
#include "../../../clientEvents/headers/NetworkEvent.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"
#include "../../../networkMessage/headers/GetMessage.h"

using namespace std;

NetworkEventStrategy::NetworkEventStrategy() : BasicEventStrategy(){ }

NetworkEventStrategy::NetworkEventStrategy(Controller* controller) : BasicEventStrategy(controller) { }

void NetworkEventStrategy::serveEvent(BasicEvent *event) {
    LOG(INFO) << "NetworkEventStrategy::serveEvent:\n" << event->toString();
    NetworkEvent *netEvent = dynamic_cast<NetworkEvent *>(event);
    shared_ptr<SimpleMessage> msg = netEvent->getMessage();

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
        case MessageType::CLIENT_CLOSE_APP :
            //nothing to do, app will be closed
            break;
    }
}

void NetworkEventStrategy::processServerInfo(SimpleMessage &message) const {
    LOG(INFO) << "NetworkEventStrategy::processServerInfo:\n" << message.toString();
    ServerInfoMessage& msg = dynamic_cast<ServerInfoMessage&>(message);
    switch (msg.getInfoType()){
        case ServerInfoMessageType::CATEGORY_CREATED :
            getModel()->addMyCategory(msg.getExtraInfo(), msg.getInfo());
            getModel()->addNotification("New category created!");
            showMainMenu();
            break;

        case ServerInfoMessageType::CATEGORY_REMOVED :
            getModel()->removeCategoryAndData(msg.getExtraInfo());
            getModel()->addNotification("One of your category removed!");
            showMainMenu();
            break;

        case ServerInfoMessageType::USER_CREATED :
            getModel()->setUserName(msg.getInfo());
            getModel()->setUserId(msg.getExtraInfo());
            getModel()->addNotification("User account created!");
            break;

        case ServerInfoMessageType::CATEGORY_JOINED :
            getModel()->addJoinedCategory(msg.getExtraInfo(), msg.getInfo());
            sendMessage(shared_ptr<GetMessage>(
                    new GetMessage(getModel()->getUserId(), GetMessageType::NEIGHBOURS, msg.getExtraInfo())));
            getModel()->addNotification("You have successfully joined the category!");
            showMainMenu();
            break;

        case ServerInfoMessageType::CATEGORY_LEFT :
            getModel()->removeCategoryAndData(msg.getExtraInfo());
            getModel()->addNotification("You have successfully left the category!");
            showMainMenu();
            break;

        case ServerInfoMessageType::CATEGORY_ACTIVATED :
            getModel()->setCategoryActive(msg.getExtraInfo(), true);
            getModel()->addNotification("Category is active again!");
            showMainMenu();
            break;

        case ServerInfoMessageType::CATEGORY_DEACTIVATED :
            getModel()->setCategoryActive(msg.getExtraInfo(), false);
            getModel()->addNotification("Category deactivated!");
            showMainMenu();
            break;

        case ServerInfoMessageType::FAIL :
            getModel()->addNotification(msg.getInfo());
            showMainMenu();
            break;
    }
}

void NetworkEventStrategy::processCategoryList(SimpleMessage &message) const {
    LOG(INFO) << "NetworkEventStrategy::processCategoryList:\n" << message.toString();
    CategoryListMessage& msg = dynamic_cast<CategoryListMessage&>(message);
    if (controller->getState() == Controller::CATEGORY_LIST) {
        getView()->showCategoryList(msg.getCategories());
        showMainMenu();
    } else if (controller->getState() == Controller::SIGN_UP) {
        getView()->showSignUpCategorySubMenu(filterCategories(msg.getCategories()));
    }
}

void NetworkEventStrategy::processNeighbourSet(SimpleMessage &message) const {
    LOG(INFO) << "NetworkEventStrategy::processNeighbourSet:\n" << message.toString();
    NeighboursInfoMessage& msg = dynamic_cast<NeighboursInfoMessage&>(message);
    getModel()->updateLeftNeighbour(msg.getCategoryId(),
                                    ConnectionInfo(msg.getLeftNeighbourIP(), msg.getLeftNeighbourPort(),
                                                   msg.getLeftNeighbourName()));
    getModel()->updateRightNeighbour(msg.getCategoryId(),
                                     ConnectionInfo(msg.getRightNeighbourIP(), msg.getRightNeighbourPort(),
                                                    msg.getRightNeighbourName()));
    stringstream ss;
    ss << "Neighbours updated: \n";
    ss << "Left neighbour: " << msg.getLeftNeighbourName() << endl;
    ss << "Right neighbour: " << msg.getRightNeighbourName() << endl;
    getModel()->addNotification(ss.str());
}

void NetworkEventStrategy::processRingMessage(SimpleMessage &message) const {
    LOG(INFO) << "NetworkEventStrategy::processRingMessage:\n" << message.toString();
    RingMessage& msg = dynamic_cast<RingMessage&>(message);
    if (getModel()->isMyCategory(msg.getCategoryId())) {
        getModel()->addMessageToInbox(msg);
        getModel()->addNotification("You have a new message!\nCheck your inbox.");
    } else {
        stringstream ss;
        ss << "Your message from " << getModel()->getCategoryName(msg.getCategoryId()) << " category has been read ";
        ss << "by all your followers!";
        getModel()->addNotification(ss.str());
    }

}

std::map<long, std::string> NetworkEventStrategy::filterCategories(
        const std::map<long, std::string> &categories) const {
    map<long, string> result = categories;
    for (pair<long, string> p : getModel()->getCategories()) {
        result.erase(p.first);
    }
    return result;
}
