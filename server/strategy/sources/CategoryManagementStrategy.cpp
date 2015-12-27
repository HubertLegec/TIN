#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/CategoryManagementStrategy.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"

void CategoryManagementStrategy::serveEvent(SimpleMessage *message) const {
    CategoryManagementMessage *categoryManagementMessage = dynamic_cast<CategoryManagementMessage *> (message);

    long senderID = categoryManagementMessage->getUserID();
    MessageType messageType = categoryManagementMessage->getMessageType();
    shared_ptr<Model> model = controller->getModel();

    if (messageType == UNDEFINED || messageType == RING_MESSAGE) {
        ServerInfoMessage *returnMessage = new ServerInfoMessage();

        returnMessage->setServerInfoMessageType(FAIL);
        returnMessage->setInfo("Undefined message.");
        returnMessage->setExtraInfo(senderID);
        returnMessage->setType(SERVER_INFO);

        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == GET) {
        controller->getStrategyMap().at(typeid(GetMessage).name())->serveEvent(message);
    } else if (messageType == CREATE_CATEGORY) {
        ServerInfoMessage *returnMessage = new ServerInfoMessage();

        try {
            model->createCategory(senderID, categoryManagementMessage->getCategoryName());
            returnMessage->setServerInfoMessageType(CATEGORY_CREATED);
        } catch (exception &exception) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }

        returnMessage->setExtraInfo(senderID);
        returnMessage->setType(SERVER_INFO);
        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == DESTROY_CATEGORY) {
        long ownerID = model->getCategoryOwner(categoryManagementMessage->getCategoryID())->getID();
        ServerInfoMessage *returnMessage = new ServerInfoMessage();

        if (ownerID != senderID) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("User not allowed to destroy category.");
        } else {
            try {
                model->destroyCategory(categoryManagementMessage->getCategoryID());
                returnMessage->setServerInfoMessageType(CATEGORY_REMOVED);
            } catch (out_of_range &exception) {
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't find category!");
            } catch (exception &exception) {
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo(exception.what());
            }
        }

        returnMessage->setExtraInfo(senderID);
        returnMessage->setType(SERVER_INFO);
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
        ServerInfoMessage *returnMessage = new ServerInfoMessage();

        try {
            model->getCategory(categoryManagementMessage->getCategoryID())->addMember(memberToAdd);
            returnMessage->setServerInfoMessageType(USER_ADDED);
        } catch (out_of_range &exception) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category!");
        } catch (runtime_error &exception) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }

        returnMessage->setExtraInfo(senderID);
        returnMessage->setType(SERVER_INFO);
        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == LEFT_CATEGORY) {
        ServerInfoMessage *returnMessage = new ServerInfoMessage();

        try {
            model->getCategory(categoryManagementMessage->getCategoryID())->removeMember(senderID);
            returnMessage->setServerInfoMessageType(CATEGORY_LEFT);
        } catch (out_of_range &exception) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category!");
        } catch (exception &exception) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }

        returnMessage->setExtraInfo(senderID);
        returnMessage->setType(SERVER_INFO);
        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == ACTIVATE_CATEGORY) {
        ServerInfoMessage *returnMessage = new ServerInfoMessage();

        try {
            model->getCategory(categoryManagementMessage->getCategoryID())->setActivated();
            returnMessage->setServerInfoMessageType(CATEGORY_ACTIVATED);
        } catch (out_of_range &exception) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category");
        } catch (exception &exception) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }

        returnMessage->setExtraInfo(senderID);
        returnMessage->setType(SERVER_INFO);
        controller->putOutgoingMessage(returnMessage);
    } else if (messageType == DEACTIVATE_CATEGORY) {
        ServerInfoMessage *returnMessage = new ServerInfoMessage();

        try {
            model->getCategory(categoryManagementMessage->getCategoryID())->setDeactivated();
            returnMessage->setServerInfoMessageType(CATEGORY_DEACTIVATED);
        } catch (out_of_range &exception) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category");
        } catch (exception &exception) {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }

        returnMessage->setExtraInfo(senderID);
        returnMessage->setType(SERVER_INFO);
        controller->putOutgoingMessage(returnMessage);
    } /*else if (messageType == NEIGHBOURS_SET) {
        // ???
    } else if (messageType == SERVER_INFO) {
        // ???
    }*/ else {
        throw runtime_error("Unsupported MessageType!");
    }
}
