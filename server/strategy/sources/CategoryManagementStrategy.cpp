#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../networkMessage/headers/GetMessage.h"
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
    returnMessage->setExtraInfo(senderID);
    returnMessage->setType(SERVER_INFO);

    if (messageType == UNDEFINED || messageType == RING_MESSAGE) {
        LOG(ERROR) << "Undefined message sent by user " << senderID;

        returnMessage->setServerInfoMessageType(FAIL);
        returnMessage->setInfo("Undefined message.");
        returnMessage->setExtraInfo(senderID);

    } else if (messageType == GET) {
        controller->getStrategyMap().at(typeid(GetMessage).name())->serveEvent(message);
    } else if (messageType == CREATE_CATEGORY) {

        auto categoryName = categoryManagementMessage->getCategoryName();
        try {
            model->createCategory(senderID, categoryName);
            LOG(INFO) << "Created category " << categoryName;
            returnMessage->setServerInfoMessageType(CATEGORY_CREATED);
        } catch (exception &exception) {
            LOG(ERROR) << "Failed to create category " << categoryName;
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }

        returnMessage->setExtraInfo(senderID);
    } else if (messageType == DESTROY_CATEGORY) {
        long categoryID = categoryManagementMessage->getCategoryID();
        long ownerID = model->getCategoryOwner(categoryID)->getID();

        if (ownerID != senderID) {
            LOG(ERROR) << "Failed to destroy category. User " << senderID << " is not an owner of category " <<
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
                    controller->putOutgoingMessage(leaveMessage);
                    tmp = tmp->getRightNeighbour();
                } while (tmp != categoryMembers);

                model->destroyCategory(categoryID);
                LOG(INFO) << "Destroyed category " << categoryID;
                returnMessage->setServerInfoMessageType(CATEGORY_REMOVED);
            } catch (out_of_range &exception) {
                LOG(ERROR) << "Failed to destroy category. Couldn't find category " << categoryID;
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't find category!");
            } catch (exception &exception) {
                LOG(ERROR) << "Failed to destroy category. Exception log: " << exception.what();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo(exception.what());
            }
        }

    }/* else if (messageType == CATEGORY_LIST) {
        map<long, std::string> categories;

        for (auto pair: controller->getModel()->getCategories()) {
            categories[pair.first] = pair.second->getName();
        }

        CategoryListMessage listMessage(-1, categories);
        controller->putOutgoingMessage(listMessage);
    }*/else if (messageType == JOIN_CATEGORY) {
        long categoryID = categoryManagementMessage->getCategoryID();
        auto memberToAdd = model->getUser(senderID);

        try {
            model->getCategory(categoryID)->addMember(memberToAdd);
            LOG(INFO) << "Added user " << senderID << " to category " << categoryID;
            returnMessage->setServerInfoMessageType(USER_ADDED);
        } catch (out_of_range &exception) {
            LOG(ERROR) << "Failed to add user " << senderID << " to category " << categoryID
            << ". Couldn't find the category";
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category!");
        } catch (runtime_error &exception) {
            LOG(ERROR) << "Failed to add user " << senderID << " to category " << categoryID << ". Exception log: "
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
            LOG(ERROR) << "User " << senderID << " failed to leave category " << categoryID
            << ". Couldn't find category";
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category!");
        } catch (exception &exception) {
            LOG(ERROR) << "User " << senderID << " failed to leave category " << categoryID
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
            LOG(ERROR) << "Couldn't activate category " << categoryID << ". Couldn't find category";
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category");
        } catch (exception &exception) {
            LOG(ERROR) << "Couldn't activate category " << categoryID << ". Exception log: " << exception.what();
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
            LOG(ERROR) << "Couldn't deactivate category " << categoryID << ". Couldn't find category";
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Couldn't find category");
        } catch (exception &exception) {
            LOG(ERROR) << "Couldn't deactivate category " << categoryID << ". Exception log: " << exception.what();
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo(exception.what());
        }
    } /*else if (messageType == NEIGHBOURS_SET) {
        // ???
    } else if (messageType == SERVER_INFO) {
        // ???
    }*/ else {
        throw runtime_error("Unsupported MessageType!");
    }

    controller->putOutgoingMessage(returnMessage);
}
