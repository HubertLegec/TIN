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

    if (messageType == CREATE_USER_ACCOUNT) {
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
    } else if (messageType == DELETE_USER_ACCOUNT || messageType == CLIENT_CLOSE_APP) {
        auto userID = managementMessage->getSenderID();
        returnMessage->setExtraInfo(userID);
        try {

            model->deleteUser(userID);
            LOG(INFO) << "Deleted user named " << userID;
            returnMessage->setServerInfoMessageType(USER_CREATED);
        } catch (exception &e) {
            LOG(ERROR) << "Failed to delete user " << userID;
            returnMessage->setServerInfoMessageType(FAIL);
        }
        controller->sendMessage(returnMessage);
    } else {
        ServerInfoMessage *returnMessage = new ServerInfoMessage(SERVER_ID, FAIL, "Bad message type received");
        returnMessage->setType(SERVER_INFO);
        LOG(DEBUG) << "Received bad message type from user " << managementMessage->getIp();
        controller->sendMessage(returnMessage, managementMessage->getIp(), managementMessage->getPort());
    }
}
