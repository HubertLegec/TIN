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
        case MessageType::NETWORK_CONTROLLER_ERROR_MESSAGE:
            processErrorMessage(*msg);
            break;
        case MessageType::EXIT :
            //app will be closed
            controller->setRunning(false);
            break;
    }
}

void NetworkEventStrategy::processServerInfo(SimpleMessage &message) const {
    LOG(INFO) << "NetworkEventStrategy::processServerInfo:\n" << message.toString();
    ServerInfoMessage& msg = dynamic_cast<ServerInfoMessage&>(message);
    switch (msg.getInfoType()){
        case ServerInfoMessageType::CATEGORY_CREATED :
            controller->incrementServerResponseNo();
            getModel()->addMyCategory(msg.getExtraInfo(), msg.getInfo());
            getModel()->addNotification("New category created!");
            showMainMenu();
            break;

        case ServerInfoMessageType::CATEGORY_REMOVED :
            controller->incrementServerResponseNo();
            getModel()->removeCategoryAndData(msg.getExtraInfo());
            getModel()->addNotification("One of your category removed!");
            showMainMenu();
            break;

        case ServerInfoMessageType::USER_CREATED :
            controller->incrementServerResponseNo();
            getModel()->setUserId(msg.getExtraInfo());
            getModel()->addNotification("User account created!");
            showMainMenu();
            break;

        case ServerInfoMessageType::CATEGORY_JOINED : {
            controller->incrementServerResponseNo();
            getModel()->addJoinedCategory(msg.getExtraInfo(), msg.getInfo());
            getModel()->addNotification("Your request to join category waits for owner confirmation.");
            showMainMenu();
            break;
        }
        case ServerInfoMessageType::CATEGORY_LEFT :
            controller->incrementServerResponseNo();
            getModel()->removeCategoryAndData(msg.getExtraInfo());
            getModel()->addNotification("You have successfully left the category!");
            showMainMenu();
            break;

        case ServerInfoMessageType::CATEGORY_ACTIVATED :
            controller->incrementServerResponseNo();
            getModel()->setCategoryActive(msg.getExtraInfo(), true);
            getModel()->addNotification("Category is active again!");
            showMainMenu();
            break;

        case ServerInfoMessageType::CATEGORY_DEACTIVATED :
            controller->incrementServerResponseNo();
            getModel()->setCategoryActive(msg.getExtraInfo(), false);
            getModel()->addNotification("Category deactivated!");
            showMainMenu();
            break;
        case ServerInfoMessageType::NEW_CATEGORY_MEMBER :
            getModel()->addPendingUser(msg.getExtraInfo(), msg.getSenderID(), msg.getInfo());
            getModel()->addNotification("User pending in one of your categories.");
            break;
        case ServerInfoMessageType::MEMBER_CONFIRMED: {
            getModel()->confirmCategory(msg.getExtraInfo());
            stringstream ss;
            ss << "Category " << getModel()->getCategoryName(msg.getExtraInfo()) <<
            " : owner confirmed your request to join!";
            getModel()->addNotification(ss.str());
            break;
        }
        case ServerInfoMessageType::MEMBER_REJECTED: {
            stringstream ss2;
            ss2 << "Category " << getModel()->getCategoryName(msg.getExtraInfo()) <<
            " : owner rejected your request to join!";
            getModel()->removeCategoryAndData(msg.getExtraInfo());
            getModel()->addNotification(ss2.str());
            break;
        }
        case ServerInfoMessageType::USER_DELETED:
            getModel()->removeUserAccount();
            getModel()->addNotification("Your account has been removed successfully.");
            showMainMenu();
        case ServerInfoMessageType::FAIL :
            controller->incrementServerResponseNo();
            getModel()->addNotification(msg.getInfo());
            showMainMenu();
            break;
    }
}

void NetworkEventStrategy::processCategoryList(SimpleMessage &message) const {
    LOG(INFO) << "NetworkEventStrategy::processCategoryList:\n" << message.toString();
    CategoryListMessage& msg = dynamic_cast<CategoryListMessage&>(message);
    if (controller->getState() == Controller::CATEGORY_LIST) {
        controller->incrementServerResponseNo();
        getView()->showCategoryList(msg.getCategories());
        showMainMenu();
    } else if (controller->getState() == Controller::SIGN_UP) {
        controller->incrementServerResponseNo();
        getView()->showSignUpCategorySubMenu(filterCategories(msg.getCategories()));
    }
}

void NetworkEventStrategy::processNeighbourSet(SimpleMessage &message) const {
    LOG(INFO) << "NetworkEventStrategy::processNeighbourSet:\n" << message.toString();
    NeighboursInfoMessage& msg = dynamic_cast<NeighboursInfoMessage&>(message);
    getModel()->updateLeftNeighbour(msg.getCategoryId(),
                                    ConnectionInfo(msg.getLeftNeighbourIP(), msg.getLeftNeighbourPort(),
                                                   msg.getLeftNeighbourName()));
    stringstream ss;
    ss << "Neighbours updated: \n";
    ss << "Left neighbour: " << msg.getLeftNeighbourName() << endl;
    ss << "Right neighbour: " << msg.getRightNeighbourName();
    getModel()->addNotification(ss.str());
}

void NetworkEventStrategy::processRingMessage(SimpleMessage &message) const {
    LOG(INFO) << "NetworkEventStrategy::processRingMessage:\n" << message.toString();
    RingMessage& msg = dynamic_cast<RingMessage&>(message);
    if (!getModel()->isMyCategory(msg.getCategoryId())) {
        getModel()->addMessageToInbox(msg);
        getModel()->addNotification("You have a new message!\nCheck your inbox.");
    } else {
        stringstream ss;
        ss << "Your message in " << getModel()->getCategoryName(msg.getCategoryId()) << " category has been read by:";
        for (string name : msg.getConfirmationsList()) {
            ss << endl << name;
        }
        getModel()->addNotification(ss.str());
    }
}

void NetworkEventStrategy::processErrorMessage(SimpleMessage &message) const {
    LOG(INFO) << "NetworkEventStrategy::processRingMessage:\n" << message.toString();
    NetworkControllerErrorMessage &msg = dynamic_cast<NetworkControllerErrorMessage &>(message);
    switch (msg.getErrorCode()) {
        case NetworkControllerErrorMessage::UNABLE_TO_CREATE_LISTENING_SOCKET:
            getView()->showInfo("ERROR!\nCan't create connection! You should try to use another settings.");
            showMainMenu();
            break;
        case NetworkControllerErrorMessage::UNABLE_TO_SEND_MSG:
            getModel()->addNotification("ERROR!\nCan't send message!");
            break;
        case NetworkControllerErrorMessage::UNABLE_TO_RESERIALIZE_MSG:
            getModel()->addNotification("ERROR!\nCan't read received message!");
            break;
        case NetworkControllerErrorMessage::UNABLE_TO_SERIALIZE_MSG:
            getModel()->addNotification("ERROR!\nProblem with sending message occured!");
            break;
        case NetworkControllerErrorMessage::UNDEFINED:
            getModel()->addNotification("ERROR!\nUndefined problem with connection occured!");
            break;
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
