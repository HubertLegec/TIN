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

                sendMessage(newUser, newUser->getID(), USER_CREATED);
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
            auto senderID = managementMessage->getSenderID();
            auto sender = controller->getModel()->getUser(senderID);
            if (!sender) {
                LOG(INFO) << "Couldn't find user who sent message. SenderID: " << senderID;
                return;
            }

            controller->getModel()->deleteUser(sender);
            LOG(INFO) << "Deleted user named " << senderID;
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
