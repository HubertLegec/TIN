#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/CategoryManagementStrategy.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"

void CategoryManagementStrategy::serveEvent(SimpleMessage *message) const {
    CategoryManagementMessage *categoryManagementMessage = dynamic_cast<CategoryManagementMessage *> (message);

    long senderID = categoryManagementMessage->getSenderID();
    MessageType messageType = categoryManagementMessage->getMessageType();
    shared_ptr<Model> model = controller->getModel();

    ServerInfoMessage *returnMessage = new ServerInfoMessage();
    returnMessage->setSenderID(SERVER_ID);
    returnMessage->setExtraInfo(senderID);
    returnMessage->setType(SERVER_INFO);

    if (messageType == UNDEFINED) {
        LOG(DEBUG) << "Undefined message sent by user " << senderID;

        returnMessage->setServerInfoMessageType(FAIL);
        returnMessage->setInfo("Undefined message.");
        returnMessage->setExtraInfo(senderID);

    } else if (messageType == CREATE_CATEGORY) {

        auto categoryName = categoryManagementMessage->getCategoryName();
        try {
            model->createCategory(senderID, categoryName);
            LOG(INFO) << "Created category " << categoryName;
            returnMessage->setServerInfoMessageType(CATEGORY_CREATED);
        } catch (exception &exception) {
            LOG(DEBUG) << "Failed to create category " << categoryName;
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Failed to create category " + categoryName);
        }
    } else if (messageType == DESTROY_CATEGORY) {
        long categoryID;
        try {
            categoryID = categoryManagementMessage->getCategoryID();
        } catch (out_of_range &e) {
            LOG(DEBUG) << "Couldn't find specified category";
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find specified category");
        } catch (exception &e) {
            LOG(DEBUG) << e.what();
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(e.what());
        }

        long ownerID = model->getCategoryOwner(categoryID)->getID();

        if (ownerID != senderID) {
            LOG(DEBUG) << "Failed to destroy category. User " << senderID << " is not an owner of category " <<
            categoryID;
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("User not allowed to destroy category.");
        } else {
            try {
                auto categoryMembers = model->getCategory(categoryID)->getMembers();
                auto tmp = categoryMembers;
                ServerInfoMessage *leaveMessage;
                do {
                    leaveMessage = new ServerInfoMessage();
                    leaveMessage->setType(SERVER_INFO);
                    leaveMessage->setServerInfoMessageType(CATEGORY_LEFT);
                    leaveMessage->setSenderID(SERVER_ID);
                    leaveMessage->setExtraInfo(tmp->getUser()->getID());

                    controller->sendMessage(leaveMessage);
                    tmp = tmp->getRightNeighbour();
                } while (tmp != categoryMembers);

                model->destroyCategory(categoryID);
                LOG(INFO) << "Destroyed category " << categoryID;
                returnMessage->setServerInfoMessageType(CATEGORY_REMOVED);
            } catch (out_of_range &exception) {
                LOG(DEBUG) << "Failed to destroy category. Couldn't find category " << categoryID;
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't find category!");
            } catch (exception &exception) {
                LOG(DEBUG) << "Failed to destroy category. Exception log: " << exception.what();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo(exception.what());
            }
        }
    } else if (messageType == JOIN_CATEGORY) {
        long categoryID = categoryManagementMessage->getCategoryID();
        auto memberToAdd = model->getUser(senderID);

        try {
            model->getCategory(categoryID)->addMember(memberToAdd);
            LOG(INFO) << "Added user " << senderID << " to category " << categoryID;
            returnMessage->setServerInfoMessageType(USER_ADDED);
        } catch (out_of_range &exception) {
            LOG(DEBUG) << "Failed to add user " << senderID << " to category " << categoryID
            << ". Couldn't find the category";
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category!");
        } catch (runtime_error &exception) {
            LOG(DEBUG) << "Failed to add user " << senderID << " to category " << categoryID << ". Exception log: "
            << exception.what();
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }
    } else if (messageType == LEFT_CATEGORY) {
        long categoryID = categoryManagementMessage->getCategoryID();

        try {
            model->getCategory(categoryID)->removeMember(senderID);
            LOG(INFO) << "User " << senderID << " left category " << categoryID;
            returnMessage->setServerInfoMessageType(CATEGORY_LEFT);
        } catch (out_of_range &exception) {
            LOG(DEBUG) << "User " << senderID << " failed to leave category " << categoryID
            << ". Couldn't find category";
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category!");
        } catch (exception &exception) {
            LOG(DEBUG) << "User " << senderID << " failed to leave category " << categoryID
            << ". Exception log: " << exception.what();
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }
    } else if (messageType == ACTIVATE_CATEGORY) {
        long categoryID = categoryManagementMessage->getCategoryID();

        try {
            model->getCategory(categoryID)->setActivated();
            LOG(INFO) << "Category " << categoryID << " activated";
            returnMessage->setServerInfoMessageType(CATEGORY_ACTIVATED);
        } catch (out_of_range &exception) {
            LOG(DEBUG) << "Couldn't activate category " << categoryID << ". Couldn't find category";
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category");
        } catch (exception &exception) {
            LOG(DEBUG) << "Couldn't activate category " << categoryID << ". Exception log: " << exception.what();
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }
    } else if (messageType == DEACTIVATE_CATEGORY) {
        long categoryID = categoryManagementMessage->getCategoryID();

        try {
            model->getCategory(categoryManagementMessage->getCategoryID())->setDeactivated();
            LOG(INFO) << "Category " << categoryID << " deactivated";
            returnMessage->setServerInfoMessageType(CATEGORY_DEACTIVATED);
        } catch (out_of_range &exception) {
            LOG(DEBUG) << "Couldn't deactivate category " << categoryID << ". Couldn't find category";
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category");
        } catch (exception &exception) {
            LOG(DEBUG) << "Couldn't deactivate category " << categoryID << ". Exception log: " << exception.what();
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }
    } else {
        returnMessage->setServerInfoMessageType(FAIL);
        returnMessage->setInfo("Bad message type received");
        LOG(DEBUG) << "Received bad message type from user " << senderID;
    }

    controller->sendMessage(returnMessage);
}
