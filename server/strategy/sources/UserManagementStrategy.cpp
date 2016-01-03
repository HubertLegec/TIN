#include "../headers/UserManagementStrategy.h"
#include "../../../networkMessage/headers/UserManagementMessage.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"

void UserManagementStrategy::serveEvent(SimpleMessage *message) const {
    UserManagementMessage *managementMessage = dynamic_cast<UserManagementMessage *>(message);

    auto messageType = managementMessage->getMessageType();
    shared_ptr<Model> model = controller->getModel();
    ServerInfoMessage *returnMessage = new ServerInfoMessage();
    returnMessage->setType(SERVER_INFO);
    returnMessage->setSenderID(SERVER_ID);

    switch (messageType) {
        case MessageType::CREATE_USER_ACCOUNT: {
            try {
                auto newUser = model->createNewUser(managementMessage->getUserName(), managementMessage->getPort(),
                                                    managementMessage->getIp());

                LOG(INFO) << "Created user named " << newUser->getName();
                returnMessage->setExtraInfo(newUser->getID());
                returnMessage->setServerInfoMessageType(USER_CREATED);

                controller->sendMessage(returnMessage);
            } catch (exception &e) {
                LOG(DEBUG) << "Failed to create user named " << managementMessage->getUserName();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't create user");

                controller->sendMessage(returnMessage, managementMessage->getIp(), managementMessage->getPort());
            }
        }
            break;

        case MessageType::DELETE_USER_ACCOUNT:
        case MessageType::CLIENT_CLOSE_APP: {
            auto userID = managementMessage->getSenderID();
            returnMessage->setExtraInfo(userID);
            try {

                model->deleteUser(userID);
                LOG(INFO) << "Deleted user named " << userID;
                returnMessage->setServerInfoMessageType(USER_CREATED);
            } catch (exception &e) {
                LOG(DEBUG) << "Failed to delete user " << userID;
                returnMessage->setServerInfoMessageType(FAIL);
            }
            controller->sendMessage(returnMessage);
        }
            break;

        default: {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Bad message type received");
            LOG(DEBUG) << "Received bad message type from user " << managementMessage->getIp();
            controller->sendMessage(returnMessage, managementMessage->getIp(), managementMessage->getPort());
        }
            break;
    }
}
