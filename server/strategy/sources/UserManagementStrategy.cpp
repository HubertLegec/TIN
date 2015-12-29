#include "../headers/UserManagementStrategy.h"
#include "../../../networkMessage/headers/UserManagementMessage.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../logger/easylogging++.h"

void UserManagementStrategy::serveEvent(SimpleMessage *message) const {
    UserManagementMessage *managementMessage = dynamic_cast<UserManagementMessage *>(message);

    auto messageType = managementMessage->getMessageType();
    shared_ptr<Model> model = controller->getModel();
    ServerInfoMessage *returnMessage = new ServerInfoMessage();
    returnMessage->setType(SERVER_INFO);

    if (messageType == USER_SIGN_UP) {
        try {
            auto newUser = model->createNewUser(managementMessage->getUserName(), managementMessage->getPort(),
                                                managementMessage->getIp());

            LOG(INFO) << "Created user named " << newUser->getName();
            returnMessage->setServerInfoMessageType(USER_CREATED);
            returnMessage->setExtraInfo(newUser->getID());
        }
        catch (exception &e) {
            LOG(ERROR) << "Failed to create user named " << managementMessage->getUserName();
            returnMessage->setServerInfoMessageType(FAIL);
            // TODO do kogo to wyslac xD? tutaj kontroler nie pobierze na podstawie USER_ID informacji na temat adresu itd...
        }
    } else if (messageType == USER_DISCONNECTED) {
        long userID = managementMessage->getSenderID();
        string name = model->getUser(userID)->getName();
        try {

            model->deleteUser(userID);
            LOG(INFO) << "Deleted user named " << name;
            returnMessage->setServerInfoMessageType(USER_CREATED);
        } catch (exception &e) {
            LOG(ERROR) << "Failed to delete user named " << name;
            returnMessage->setServerInfoMessageType(FAIL);
        }
        returnMessage->setExtraInfo(userID);
    }

    controller->sendMessage(returnMessage);
}
