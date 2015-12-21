#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../headers/CategoryManagementStrategy.h"

void CategoryManagementStrategy::serveEvent(SimpleMessage *message) {
    CategoryManagementMessage *categoryManagementMessage = (CategoryManagementMessage *) (message);

    long senderID = categoryManagementMessage->getUserID();
    MessageType messageType = categoryManagementMessage->getMessageType();
    shared_ptr<Model> model = controller->getModel();

    if (messageType == UNDEFINED) {
        // nothing to do
    } else if (messageType == GET) {
        // ???
    } else if (messageType == CREATE_CATEGORY) {
        model->createCategory(senderID, categoryManagementMessage->getCategoryName());
    } else if (messageType == DESTROY_CATEGORY) {
        long ownerID = model->getCategoryOwner(categoryManagementMessage->getCategoryID())->getID();

        if (ownerID != senderID) {
            // TODO
            // wyslac blad
        } else {
            model->destroyCategory(categoryManagementMessage->getCategoryID());
        }
    } else if (messageType == CATEGORY_LIST) {

    } else if (messageType == JOIN_CATEGORY) {
        auto memberToAdd = model->getUser(senderID);
        model->getCategory(categoryManagementMessage->getCategoryID())->addMember(memberToAdd);
    } else if (messageType == LEFT_CATEGORY) {
        model->getCategory(categoryManagementMessage->getCategoryID())->removeMember(senderID);
    } else if (messageType == ACTIVATE_CATEGORY) {
        model->getCategory(categoryManagementMessage->getCategoryID())->setActivated();
    } else if (messageType == DEACTIVATE_CATEGORY) {
        model->getCategory(categoryManagementMessage->getCategoryID())->setDeactivated();
    } else if (messageType == RING_MESSAGE) {
        // ???
    } else if (messageType == NEIGHBOURS_SET) {
        // ???
    } else if (messageType == SERVER_INFO) {
        // ???
    } else {
        throw runtime_error("Unsupported MessageType!");
    }
}
