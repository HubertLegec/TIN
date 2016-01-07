#include "../headers/UserManagementStrategy.h"
#include "../../../networkMessage/headers/UserManagementMessage.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"
#include "../../utils/ServerGlobalConstants.h"

void UserManagementStrategy::serveEvent(SimpleMessage *message) const {
    UserManagementMessage *managementMessage = dynamic_cast<UserManagementMessage *>(message);

    auto messageType = managementMessage->getMessageType();
    switch (messageType) {
        case MessageType::CREATE_USER_ACCOUNT: {
            createUserAccount(managementMessage->getUserName(), managementMessage->getIp(),
                              managementMessage->getPort());
        }
            break;

        case MessageType::DELETE_USER_ACCOUNT:
        case MessageType::CLIENT_CLOSE_APP: {
            if (checkSender(managementMessage) == ServerGlobalConstant::FAILED_CODE)
                return;

            deleteUser();
        }
            break;

        default: {
            badMessageTypeReceived(managementMessage->getIp(), managementMessage->getPort());
        }
            break;
    }
}

void UserManagementStrategy::createUserAccount(const string &userName, const string &userIP, int userPort) const {
    if (auto newUser = getModel()->createNewUser(userName, userPort, userIP)) {
        sendMessage(newUser, newUser->getID(), ServerInfoMessageType::USER_CREATED);
    } else {
        LOG(INFO) << "Failed to create user named " << userName;
        ServerInfoMessage *returnMessage = new ServerInfoMessage(ServerGlobalConstant::SERVER_ID,
                                                                 ServerInfoMessageType::FAIL, "Couldn't create user!");
        controller->sendMessage(returnMessage, userIP, userPort);
    }
}

void UserManagementStrategy::deleteUser() const {
    list<shared_ptr<Category> > categoriesToRemove;
    for (auto pair : controller->getModel()->getCategories()) {
        if (pair.second->getOwner() == getSender()) {
            sendForAllMembers(pair.second, pair.first, ServerInfoMessageType::CATEGORY_REMOVED);
            categoriesToRemove.push_back(pair.second);
        } else if (auto member = pair.second->findMember(getSender()->getID())) {
            auto leftNeighbour = member->getLeftNeighbour();
            auto rightNeighbour = member->getRightNeighbour();

            pair.second->removeMember(getSender()->getID());
            LOG(INFO) << "User " << getSender()->getID() << " left category " << pair.first;

            sendNeighbours(pair.second, leftNeighbour);
            sendNeighbours(pair.second, rightNeighbour);
            sendMessage(getSender(), pair.first, ServerInfoMessageType::CATEGORY_LEFT);
        }
    }

    for (auto categoryToRemove : categoriesToRemove) {
        getModel()->deleteCategory(categoryToRemove);
        LOG(INFO) << "Deleted category " << categoryToRemove->getName();
    }

    controller->getModel()->deleteUser(getSender());
    LOG(INFO) << "Deleted user named " << getSender()->getID();
}
