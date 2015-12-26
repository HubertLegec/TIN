#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/CategoryManagementStrategy.h"

void CategoryManagementStrategy::serveEvent(SimpleMessage *message) const {
    CategoryManagementMessage *categoryManagementMessage = dynamic_cast<CategoryManagementMessage *> (message);

    long senderID = categoryManagementMessage->getUserID();
    MessageType messageType = categoryManagementMessage->getMessageType();
    shared_ptr<Model> model = controller->getModel();

    if (messageType == UNDEFINED || messageType == RING_MESSAGE) {
        ServerInfoMessage returnMessage;

        returnMessage.setServerInfoMessageType(FAIL);
        returnMessage.setInfo("Undefined message.");
        returnMessage.setExtraInfo(senderID);

        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == GET) {
        controller->getStrategyMap().at(typeid(GetMessage).name()).serveEvent(message);
    } else if (messageType == CREATE_CATEGORY) {
        ServerInfoMessage returnMessage;

        model->createCategory(senderID, categoryManagementMessage->getCategoryName());
        returnMessage.setServerInfoMessageType(CATEGORY_CREATED);
        returnMessage.setExtraInfo(senderID);

        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == DESTROY_CATEGORY) {
        long ownerID = model->getCategoryOwner(categoryManagementMessage->getCategoryID())->getID();
        ServerInfoMessage returnMessage;

        if (ownerID != senderID) {
            returnMessage.setServerInfoMessageType(FAIL);
            returnMessage.setInfo("User not allowed to destroy category.");
        } else {
            model->destroyCategory(categoryManagementMessage->getCategoryID());
            returnMessage.setServerInfoMessageType(CATEGORY_REMOVED);
        }
        returnMessage.setExtraInfo(senderID);

        controller->putOutgoingMessage(returnMessage);
    }/* else if (messageType == CATEGORY_LIST) {
        map<long, std::string> categories;

        for (auto pair: controller->getModel()->getCategories()) {
            categories[pair.first] = pair.second->getName();
        }

        CategoryListMessage listMessage(-1, categories);
        controller->putOutgoingMessage(listMessage);
    }*/else if (messageType == JOIN_CATEGORY) {
        auto memberToAdd = model->getUser(senderID);
        ServerInfoMessage returnMessage;

        model->getCategory(categoryManagementMessage->getCategoryID())->addMember(memberToAdd);
        returnMessage.setExtraInfo(senderID);
        returnMessage.setServerInfoMessageType(USER_ADDED);
        returnMessage.setExtraInfo(senderID);

        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == LEFT_CATEGORY) {
        ServerInfoMessage returnMessage;

        model->getCategory(categoryManagementMessage->getCategoryID())->removeMember(senderID);
        returnMessage.setExtraInfo(senderID);
        returnMessage.setServerInfoMessageType(CATEGORY_LEFT);
        returnMessage.setExtraInfo(senderID);

        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == ACTIVATE_CATEGORY) {
        ServerInfoMessage returnMessage;

        model->getCategory(categoryManagementMessage->getCategoryID())->setActivated();
        returnMessage.setServerInfoMessageType(CATEGORY_ACTIVATED);
        returnMessage.setExtraInfo(senderID);

        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == DEACTIVATE_CATEGORY) {
        ServerInfoMessage returnMessage;

        model->getCategory(categoryManagementMessage->getCategoryID())->setDeactivated();
        returnMessage.setServerInfoMessageType(CATEGORY_DEACTIVATED);
        returnMessage.setExtraInfo(senderID);

        controller->putOutgoingMessage(returnMessage);
    } /*else if (messageType == NEIGHBOURS_SET) {
        // ???
    } else if (messageType == SERVER_INFO) {
        // ???
    }*/ else {
        throw runtime_error("Unsupported MessageType!");
    }
}
