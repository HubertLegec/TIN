#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../headers/CategoryManagementStrategy.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"

void CategoryManagementStrategy::serveEvent(SimpleMessage *message) const {
    CategoryManagementMessage *categoryManagementMessage = dynamic_cast<CategoryManagementMessage *> (message);

    long senderID = categoryManagementMessage->getSenderID();
    long categoryID = categoryManagementMessage->getCategoryID();
    MessageType messageType = categoryManagementMessage->getMessageType();
    shared_ptr<Model> model = controller->getModel();

    switch (messageType) {
        case MessageType::UNDEFINED: {
            LOG(DEBUG) << "Undefined message sent by user " << senderID;
            sendMessage(senderID, -1, FAIL, "Undefined message");
        }
            break;

        case MessageType::CREATE_CATEGORY: {

            auto categoryName = categoryManagementMessage->getCategoryName();
            try {
                categoryID = model->createCategory(senderID, categoryName);
                LOG(INFO) << "Created category " << categoryName;
                sendMessage(senderID, categoryID, CATEGORY_CREATED);
                sendNeighbours(categoryID, senderID);
            } catch (exception &exception) {
                LOG(DEBUG) << "Failed to create category " << categoryName;
                sendMessage(senderID, -1, FAIL, "Failed to create category " + categoryName);
            }
        }
            break;

        case MessageType::DESTROY_CATEGORY: {
            long ownerID;
            try {
                ownerID = model->getCategoryOwner(categoryID)->getID();
            } catch (out_of_range &e) {
                LOG(DEBUG) << "Couldn't find specified category";
                sendMessage(senderID, categoryID, FAIL, "Couldn't find specified category");
            } catch (exception &e) {
                LOG(DEBUG) << e.what();
                sendMessage(senderID, categoryID, FAIL, e.what());
            }

            if (ownerID != senderID) {
                LOG(DEBUG) << "Failed to destroy category. User " << senderID << " is not an owner of category " <<
                categoryID;
                sendMessage(senderID, categoryID, FAIL, "User not allowed to destroy category.");
            } else {
                try {
                    ServerInfoMessage *leaveMessage = new ServerInfoMessage(SERVER_ID, CATEGORY_REMOVED, "");
                    leaveMessage->setExtraInfo(categoryID);
                    sendForAllMembers(categoryID, leaveMessage);
                    model->destroyCategory(categoryID);
                    LOG(INFO) << "Destroyed category " << categoryID;
                } catch (out_of_range &e) {
                    LOG(DEBUG) << "Failed to destroy category. Couldn't find category " << categoryID;
                    sendMessage(senderID, categoryID, FAIL, "Couldn't find category!");
                } catch (exception &e) {
                    LOG(DEBUG) << "Failed to destroy category. Exception log: " << e.what();
                    sendMessage(senderID, categoryID, FAIL, e.what());
                }
            }
        }
            break;

        case MessageType::JOIN_CATEGORY: {
            try {
                auto userToAdd = model->getUser(senderID);
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                category->addNewMember(userToAdd);
                LOG(INFO) << "Added user " << senderID << " to category " << categoryID;
                sendMessage(senderID, categoryID, CATEGORY_JOINED);
                //client needs userID, so it is send as sendrer id instead of SERVER_ID
                ServerInfoMessage *newMemberMessage = new ServerInfoMessage(userToAdd->getID(), NEW_CATEGORY_MEMBER,
                                                                            userToAdd->getName() + " " +
                                                                            userToAdd->getIP());
                newMemberMessage->setExtraInfo(categoryID);
                controller->sendMessage(newMemberMessage, owner->getID());
            } catch (out_of_range &e) {
                LOG(DEBUG) << "Failed to add user " << senderID << " to category " << categoryID
                << ". Couldn't find the category";
                sendMessage(senderID, categoryID, FAIL, "Couldn't find category!");
            } catch (runtime_error &e) {
                LOG(DEBUG) << "Failed to add user " << senderID << " to category " << categoryID << ". Exception log: "
                << e.what();
                sendMessage(senderID, categoryID, FAIL, e.what());
            }
        }
            break;

        case MessageType::LEFT_CATEGORY: {
            try {
                auto memberToRemove = model->getCategory(categoryID)->findMember(senderID);
                auto leftNeighbourID = memberToRemove->getLeftNeighbour()->getUser()->getID();
                auto rightNeighbourID = memberToRemove->getRightNeighbour()->getUser()->getID();

                model->getCategory(categoryID)->removeMember(senderID);
                LOG(INFO) << "User " << senderID << " left category " << categoryID;

                sendNeighbours(categoryID, leftNeighbourID);
                sendNeighbours(categoryID, rightNeighbourID);

                sendMessage(senderID, categoryID, CATEGORY_LEFT);
            } catch (out_of_range &e) {
                LOG(DEBUG) << "User " << senderID << " failed to leave category " << categoryID
                << ". Couldn't find category";
                sendMessage(senderID, categoryID, FAIL, "Couldn't find category!");
            } catch (exception &e) {
                LOG(DEBUG) << "User " << senderID << " failed to leave category " << categoryID
                << ". Exception log: " << e.what();
                sendMessage(senderID, categoryID, FAIL, e.what());
            }
        }
            break;

        case MessageType::ACTIVATE_CATEGORY: {
            try {
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                ServerInfoMessage *returnMessage = new ServerInfoMessage(SERVER_ID, CATEGORY_ACTIVATED, "");
                returnMessage->setExtraInfo(categoryID);
                if (senderID == owner->getID()) {
                    category->setActivated();
                    sendForAllMembers(categoryID, returnMessage);
                    LOG(INFO) << "Category " << categoryID << " activated";
                } else if (category->isUnconfirmed(senderID)) {
                    sendMessage(senderID, categoryID, FAIL, "You are unconfirmed. You can't become online!");
                } else {
                    auto member = category->findMember(senderID);
                    member->setStatus(ONLINE);
                    sendMessage(senderID, categoryID, CATEGORY_ACTIVATED);
                    sendAllNeighbours(categoryID, member->getUser()->getID());
                }
            } catch (out_of_range &e) {
                LOG(DEBUG) << "Couldn't activate category " << categoryID << ". Couldn't find category";
                sendMessage(senderID, categoryID, FAIL, "Couldn't find category");
            } catch (exception &e) {
                LOG(DEBUG) << "Couldn't activate category " << categoryID << ". Exception log: " << e.what();
                sendMessage(senderID, categoryID, FAIL, e.what());
            }
        }
            break;

        case MessageType::DEACTIVATE_CATEGORY: {
            try {
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                ServerInfoMessage *returnMessage = new ServerInfoMessage(SERVER_ID, CATEGORY_DEACTIVATED, "");
                returnMessage->setExtraInfo(categoryID);
                if (senderID == owner->getID()) {
                    category->setDeactivated();
                    sendForAllMembers(categoryID, returnMessage);
                    LOG(INFO) << "Category " << categoryID << " deactivated";
                } else if (category->isUnconfirmed(senderID)) {
                    sendMessage(senderID, categoryID, FAIL, "You are unconfirmed. You can't become online!");
                } else {
                    auto member = category->findMember(senderID);
                    member->setStatus(OFFLINE);
                    sendMessage(senderID, categoryID, CATEGORY_DEACTIVATED);
                    sendAllNeighbours(categoryID, member->getUser()->getID());
                }
            } catch (out_of_range &e) {
                LOG(DEBUG) << "Couldn't deactivate category " << categoryID << ". Couldn't find category";
                sendMessage(senderID, categoryID, FAIL, "Couldn't find category");

            } catch (exception &e) {
                LOG(DEBUG) << "Couldn't deactivate category " << categoryID << ". Exception log: " <<
                e.what();
                sendMessage(senderID, categoryID, FAIL, e.what());
            }
        }
            break;

        case NEW_MEMBER_CONFIRM: {
            long memberID = categoryManagementMessage->getExtraInfo();

            try {
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                if (owner->getID() != senderID) {
                    sendMessage(senderID, categoryID, FAIL, "User is not an owner of category " + categoryID);
                } else {
                    if (category->isUnconfirmed(memberID)) {
                        sendMessage(senderID, categoryID, FAIL,
                                    "Member is already confirmed or didn't join the category");
                    } else {
                        sendMessage(senderID, categoryID, OK, "Activated user " + categoryID);
                        category->acceptNewUser(memberID);
                        sendMessage(memberID, categoryID, MEMBER_CONFIRMED);
                        auto member = category->findMember(memberID);
                        sendAllNeighbours(categoryID, member->getUser()->getID());
                    }
                }

            } catch (out_of_range &e) {
                LOG(DEBUG) << "Couldn't confirm new member. Couldn't find category " << categoryID;
                sendMessage(senderID, categoryID, FAIL, "Couldn't confirm new member. Couldn't find category ");

            } catch (exception &e) {
                LOG(DEBUG) << "Couldn't confirm new member " << categoryID << ". Exception log: " <<
                e.what();
                sendMessage(senderID, categoryID, FAIL, e.what());
            }
        }
            break;

        case NEW_MEMBER_REJECT: {
            long memberID = categoryManagementMessage->getExtraInfo();

            try {
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                if (owner->getID() != senderID) {
                    sendMessage(senderID, categoryID, FAIL, "User is not an owner of category " + categoryID);
                } else {
                    if (category->isUnconfirmed(memberID)) {
                        sendMessage(senderID, categoryID, FAIL,
                                    "Member is already confirmed or didn't join the category");
                    } else {
                        category->rejectMember(memberID);
                        sendMessage(senderID, categoryID, OK, "Rejected user " + categoryID);
                        sendMessage(memberID, categoryID, MEMBER_REJECTED, "Rejected user in category " + categoryID);
                    }
                }

            } catch (out_of_range &e) {
                LOG(DEBUG) << "Couldn't refect new member. Couldn't find category " << categoryID;
                sendMessage(senderID, categoryID, FAIL, "Couldn't find category");

            } catch (exception &e) {
                LOG(DEBUG) << "Couldn't reject new member " << categoryID << ". Exception log: " <<
                e.what();
                sendMessage(senderID, categoryID, FAIL, e.what());
            }
        }
            break;

        default: {
            LOG(DEBUG) << "Received bad message type from user " << senderID;
            sendMessage(senderID, -1, FAIL, "Bad message type received");
        }
            break;
    }
}