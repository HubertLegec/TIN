#include "../headers/UserManagementStrategy.h"
#include "../../../networkMessage/headers/UserManagementMessage.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"

void UserManagementStrategy::serveEvent(SimpleMessage *message) const {
    UserManagementMessage *managementMessage = dynamic_cast<UserManagementMessage *>(message);

    auto messageType = managementMessage->getMessageType();
    shared_ptr<Model> model = controller->getModel();

    switch (messageType) {
        case MessageType::CREATE_USER_ACCOUNT: {
            try {
                auto newUser = model->createNewUser(managementMessage->getUserName(), managementMessage->getPort(),
                                                    managementMessage->getIp());

                sendMessage(newUser->getID(), newUser->getID(), USER_CREATED);
            } catch (exception &e) {
                LOG(DEBUG) << "Failed to create user named " << managementMessage->getUserName();
                ServerInfoMessage *returnMessage = new ServerInfoMessage(SERVER_ID, FAIL, "Couldn't create user!");
                controller->sendMessage(returnMessage, managementMessage->getIp(), managementMessage->getPort());
            }
        }
            break;

        case MessageType::DELETE_USER_ACCOUNT:
        case MessageType::CLIENT_CLOSE_APP: {
            // TODO usuwanie z kategorii itd...
            auto userID = managementMessage->getSenderID();
            try {

                LOG(INFO) << "Deleted user named " << userID;
            } catch (exception &e) {
                LOG(DEBUG) << "Failed to delete user " << userID;
                sendMessage(userID, FAIL, "Failed to delete user!");
            }
        }
            break;

        default: {
            LOG(DEBUG) << "Received bad message type from user " << managementMessage->getIp();
            ServerInfoMessage *returnMessage = new ServerInfoMessage(SERVER_ID, FAIL, "Bad message type received!");
            controller->sendMessage(returnMessage, managementMessage->getIp(), managementMessage->getPort());
        }
            break;
    }
}
