#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../headers/CategoryManagementStrategy.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"

void CategoryManagementStrategy::serveEvent(SimpleMessage *message) const {
    CategoryManagementMessage *categoryManagementMessage = dynamic_cast<CategoryManagementMessage *> (message);

    shared_ptr<Model> model = controller->getModel();
    long senderID = categoryManagementMessage->getSenderID();
    auto sender = model->getUser(senderID);
    if (!sender) {
        LOG(INFO) << "Couldn't find user who sent message. SenderID: " << senderID;
        return;
    }

    long categoryID = categoryManagementMessage->getCategoryID();
    MessageType messageType = categoryManagementMessage->getMessageType();

    switch (messageType) {
        case MessageType::UNDEFINED: {
            LOG(DEBUG) << "Undefined message sent by user " << senderID;
            sendMessage(sender, FAILED_CODE, FAIL, "Undefined message");
        }
            break;

        case MessageType::CREATE_CATEGORY: {

            auto categoryName = categoryManagementMessage->getCategoryName();
            categoryID = model->createCategory(senderID, categoryName);

            if (categoryID == FAILED_CODE) {
                sendMessage(sender, FAILED_CODE, FAIL, "Failed to create category " + categoryName);
            } else {
                LOG(INFO) << "Created category " << categoryName;
                sendMessage(sender, categoryID, CATEGORY_CREATED, categoryName);
                auto category = model->getCategory(categoryID);
                sendNeighbours(category, category->getMembers());
            }
        }
            break;

        case MessageType::DESTROY_CATEGORY: {
            auto category = model->getCategory(categoryID);
            if (!category) {
                sendCategoryNotFound(sender, categoryID);
                break;
            }

            auto owner = category->getOwner();
            if (!owner) {
                sendCategoryNotFound(sender, categoryID);
                break;
            }

            if (owner->getID() != senderID) {
                LOG(INFO) << "Failed to destroy category. User " << senderID << " is not an owner of category " <<
                categoryID;
                sendMessage(sender, categoryID, FAIL, "User not allowed to destroy category.");
            } else {
                sendForAllMembers(category, categoryID, CATEGORY_REMOVED);
                model->destroyCategory(category);
                LOG(INFO) << "Destroyed category " << categoryID;
            }
        }
            break;

        case MessageType::JOIN_CATEGORY: {
            auto userToAdd = model->getUser(senderID);
            if (!userToAdd) {
                sendMemberNotFound(sender, categoryID, senderID);
                break;
            }

            auto category = model->getCategory(categoryID);
            if (!category) {
                sendCategoryNotFound(sender, categoryID);
                break;
            }
            auto owner = category->getOwner();
            category->addNewMember(userToAdd);
            LOG(INFO) << "Added user " << sender << " to category " << categoryID;
            sendMessage(sender, categoryID, CATEGORY_JOINED);

            //client needs userID, so it is send as sendrer id instead of SERVER_ID
            ServerInfoMessage *newMemberMessage = new ServerInfoMessage(userToAdd->getID(), NEW_CATEGORY_MEMBER,
                                                                        userToAdd->getName() + " " +
                                                                        userToAdd->getIP());
            newMemberMessage->setExtraInfo(categoryID);
            controller->sendMessage(newMemberMessage, owner->getID());
        }
            break;

        case MessageType::LEFT_CATEGORY: {
            auto category = model->getCategory(categoryID);
            if (!category) {
                sendCategoryNotFound(sender, categoryID);
                break;
            }

            auto memberToRemove = category->findMember(senderID);
            if (!memberToRemove) {
                sendMemberNotFound(sender, categoryID, senderID);
                break;
            }

            auto leftNeighbour = memberToRemove->getLeftNeighbour();
            auto rightNeighbour = memberToRemove->getRightNeighbour();

            category->removeMember(senderID);
            LOG(INFO) << "User " << senderID << " left category " << categoryID;

            sendNeighbours(category, leftNeighbour);
            sendNeighbours(category, rightNeighbour);

            sendMessage(sender, categoryID, CATEGORY_LEFT);
        }
            break;

        case MessageType::ACTIVATE_CATEGORY: {
            auto category = model->getCategory(categoryID);
            if (!category) {
                sendCategoryNotFound(sender, categoryID);
                break;
            }
            auto owner = category->getOwner();

            if (senderID == owner->getID()) {
                category->setActivated();
                sendForAllMembers(category, categoryID, CATEGORY_DEACTIVATED);
                LOG(INFO) << "Category " << categoryID << " activated";
            } else if (category->isUnconfirmed(senderID)) {
                sendMessage(sender, categoryID, FAIL, "You are unconfirmed. You can't become online!");
            } else {
                auto member = category->findMember(senderID);
                if (!member) {
                    sendMemberNotFound(sender, categoryID, senderID);
                    break;
                }
                member->setStatus(ONLINE);
                sendMessage(sender, categoryID, CATEGORY_ACTIVATED);
                sendAllNeighbours(category, member);
            }
        }
            break;

        case MessageType::DEACTIVATE_CATEGORY: {
            auto category = model->getCategory(categoryID);
            if (!category) {
                sendCategoryNotFound(sender, categoryID);
                break;
            }
            auto owner = category->getOwner();

            if (senderID == owner->getID()) {
                category->setDeactivated();
                sendForAllMembers(category, categoryID, CATEGORY_DEACTIVATED);
                LOG(INFO) << "Category " << categoryID << " deactivated";
            } else if (category->isUnconfirmed(senderID)) {
                sendMessage(sender, categoryID, FAIL, "You are unconfirmed. You can't become offline!");
            } else {
                auto member = category->findMember(senderID);
                member->setStatus(OFFLINE);
                sendMessage(sender, categoryID, CATEGORY_DEACTIVATED);
                sendAllNeighbours(category, member);
            }
        }
            break;

        case NEW_MEMBER_CONFIRM: {
            long memberID = categoryManagementMessage->getExtraInfo();
            auto category = model->getCategory(categoryID);
            if (!category) {
                sendCategoryNotFound(sender, categoryID);
                break;
            }
            auto owner = category->getOwner();
            if (owner->getID() != senderID) {
                sendMessage(sender, categoryID, FAIL, "User is not an owner of category " + categoryID);
            } else {
                if (category->isUnconfirmed(memberID)) {
                    sendMessage(sender, categoryID, OK, "Activated user " + categoryID);
                    category->acceptNewUser(memberID);
                    auto member = category->findMember(memberID);
                    sendMessage(member->getUser(), categoryID, MEMBER_CONFIRMED);
                    sendAllNeighbours(category, member);

                } else {
                    sendMessage(sender, categoryID, FAIL,
                                "Member is already confirmed or didn't join the category");
                }
            }
        }
            break;

        case NEW_MEMBER_REJECT: {
            auto category = model->getCategory(categoryID);
            if (!category) {
                sendCategoryNotFound(sender, categoryID);
                break;
            }
            auto owner = category->getOwner();
            long memberID = categoryManagementMessage->getExtraInfo();
            auto member = category->findMember(memberID);
            if (!member) {
                sendMemberNotFound(sender, categoryID, memberID);
            }

            if (owner->getID() != senderID) {
                sendMessage(sender, categoryID, FAIL, "User is not an owner of category " + categoryID);
            } else {
                if (category->isUnconfirmed(memberID)) {
                    category->rejectMember(memberID);
                    sendMessage(sender, categoryID, OK, "Rejected user " + categoryID);
                    sendMessage(member->getUser(), categoryID, MEMBER_REJECTED,
                                "Rejected user in category " + categoryID);
                } else {
                    sendMessage(sender, categoryID, FAIL,
                                "Member is already confirmed or didn't join the category");
                }
            }
        }
            break;

        default: {
            LOG(DEBUG) << "Received bad message type from user " << senderID;
            sendMessage(sender, FAILED_CODE, FAIL, "Bad message type received");
        }
            break;
    }
}