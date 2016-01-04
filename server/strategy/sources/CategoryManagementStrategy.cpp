#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/CategoryManagementStrategy.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"

void CategoryManagementStrategy::serveEvent(SimpleMessage *message) const {
    CategoryManagementMessage *categoryManagementMessage = dynamic_cast<CategoryManagementMessage *> (message);

    long senderID = categoryManagementMessage->getSenderID();
    MessageType messageType = categoryManagementMessage->getMessageType();
    shared_ptr<Model> model = controller->getModel();

    ServerInfoMessage *returnMessage = new ServerInfoMessage();
    returnMessage->setSenderID(SERVER_ID);
    returnMessage->setType(SERVER_INFO);

    switch (messageType) {
        case MessageType::UNDEFINED: {
            LOG(DEBUG) << "Undefined message sent by user " << senderID;

            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Undefined message.");
            returnMessage->setExtraInfo(senderID);
        }
            controller->sendMessage(returnMessage, senderID);
            break;

        case MessageType::CREATE_CATEGORY: {

            auto categoryName = categoryManagementMessage->getCategoryName();
            try {
                long categoryID = model->createCategory(senderID, categoryName);
                LOG(INFO) << "Created category " << categoryName;

                returnMessage->setServerInfoMessageType(CATEGORY_CREATED);
                returnMessage->setExtraInfo(categoryID);
                returnMessage->setInfo(categoryManagementMessage->getCategoryName());
                controller->sendMessage(returnMessage, senderID);

                sendNeighbours(categoryID, senderID);
            } catch (exception &exception) {
                LOG(DEBUG) << "Failed to create category " << categoryName;
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Failed to create category " + categoryName);
                controller->sendMessage(returnMessage, senderID);
            }
        }
            break;

        case MessageType::DESTROY_CATEGORY: {
            long categoryID = categoryManagementMessage->getCategoryID();
            long ownerID;
            try {
                ownerID = model->getCategoryOwner(categoryID)->getID();
            } catch (out_of_range &e) {
                LOG(DEBUG) << "Couldn't find specified category";
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setExtraInfo(categoryID);
                returnMessage->setInfo("Couldn't find specified category");
                controller->sendMessage(returnMessage, senderID);
            } catch (exception &e) {
                LOG(DEBUG) << e.what();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setExtraInfo(categoryID);
                returnMessage->setInfo(e.what());
                controller->sendMessage(returnMessage, senderID);
            }

            if (ownerID != senderID) {
                LOG(DEBUG) << "Failed to destroy category. User " << senderID << " is not an owner of category " <<
                categoryID;
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setExtraInfo(categoryID);
                returnMessage->setInfo("User not allowed to destroy category.");
                controller->sendMessage(returnMessage, senderID);
            } else {
                try {
                    auto categoryMembers = model->getCategory(categoryID)->getMembers();
                    auto tmp = categoryMembers;
                    ServerInfoMessage *leaveMessage;
                    do {
                        leaveMessage = new ServerInfoMessage();
                        leaveMessage->setType(SERVER_INFO);
                        leaveMessage->setServerInfoMessageType(CATEGORY_REMOVED);
                        leaveMessage->setSenderID(SERVER_ID);
                        leaveMessage->setExtraInfo(categoryID);

                        controller->sendMessage(leaveMessage, senderID);
                        tmp = tmp->getRightNeighbour();
                    } while (tmp != categoryMembers);

                    model->destroyCategory(categoryID);
                    LOG(INFO) << "Destroyed category " << categoryID;
                } catch (out_of_range &exception) {
                    LOG(DEBUG) << "Failed to destroy category. Couldn't find category " << categoryID;
                    returnMessage->setServerInfoMessageType(FAIL);
                    returnMessage->setExtraInfo(categoryID);
                    returnMessage->setInfo("Couldn't find category!");
                    controller->sendMessage(returnMessage, senderID);
                } catch (exception &exception) {
                    LOG(DEBUG) << "Failed to destroy category. Exception log: " << exception.what();
                    returnMessage->setServerInfoMessageType(FAIL);
                    returnMessage->setExtraInfo(categoryID);
                    returnMessage->setInfo(exception.what());
                    controller->sendMessage(returnMessage, senderID);
                }
            }
        }
            break;

        case MessageType::JOIN_CATEGORY: {
            long categoryID = categoryManagementMessage->getCategoryID();
            returnMessage->setExtraInfo(categoryID);
            auto userToAdd = model->getUser(senderID);

            try {
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                category->addMember(userToAdd);
                LOG(INFO) << "Added user " << senderID << " to category " << categoryID;

                returnMessage->setInfo(category->getName());
                returnMessage->setServerInfoMessageType(CATEGORY_JOINED);
                controller->sendMessage(returnMessage, senderID);

                ServerInfoMessage *newMemberMessage = new ServerInfoMessage();
                newMemberMessage->setSenderID(SERVER_ID);
                newMemberMessage->setType(SERVER_INFO);
                newMemberMessage->setServerInfoMessageType(NEW_CATEGORY_MEMBER);
                newMemberMessage->setExtraInfo(categoryID);
                newMemberMessage->setInfo(userToAdd->getName() + " " + userToAdd->getIP());
                controller->sendMessage(newMemberMessage, owner->getID());

                auto newMember = category->findMember(userToAdd->getID());
                auto leftNeighbourID = newMember->getLeftNeighbour()->getUser()->getID();
                auto rightNeighbourID = newMember->getRightNeighbour()->getUser()->getID();

                sendNeighbours(categoryID, newMember->getUser()->getID());
                sendNeighbours(categoryID, leftNeighbourID);
                sendNeighbours(categoryID, rightNeighbourID);
            } catch (out_of_range &exception) {
                LOG(DEBUG) << "Failed to add user " << senderID << " to category " << categoryID
                << ". Couldn't find the category";
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't find category!");
                controller->sendMessage(returnMessage, senderID);
            } catch (runtime_error &exception) {
                LOG(DEBUG) << "Failed to add user " << senderID << " to category " << categoryID << ". Exception log: "
                << exception.what();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo(exception.what());
                controller->sendMessage(returnMessage, senderID);
            }
        }
            break;

        case MessageType::LEFT_CATEGORY: {
            long categoryID = categoryManagementMessage->getCategoryID();
            returnMessage->setExtraInfo(categoryID);

            try {
                auto memberToRemove = model->getCategory(categoryID)->findMember(senderID);
                auto leftNeighbourID = memberToRemove->getLeftNeighbour()->getUser()->getID();
                auto rightNeighbourID = memberToRemove->getRightNeighbour()->getUser()->getID();

                model->getCategory(categoryID)->removeMember(senderID);
                LOG(INFO) << "User " << senderID << " left category " << categoryID;

                sendNeighbours(categoryID, leftNeighbourID);
                sendNeighbours(categoryID, rightNeighbourID);

                returnMessage->setServerInfoMessageType(CATEGORY_LEFT);
            } catch (out_of_range &exception) {
                LOG(DEBUG) << "User " << senderID << " failed to leave category " << categoryID
                << ". Couldn't find category";
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't find category!");
            } catch (exception &exception) {
                LOG(DEBUG) << "User " << senderID << " failed to leave category " << categoryID
                << ". Exception log: " << exception.what();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo(exception.what());
            }
        }
            controller->sendMessage(returnMessage, senderID);
            break;

        case MessageType::ACTIVATE_CATEGORY: {
            long categoryID = categoryManagementMessage->getCategoryID();
            returnMessage->setExtraInfo(categoryID);

            try {
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                returnMessage->setServerInfoMessageType(CATEGORY_ACTIVATED);
                if (senderID == owner->getID()) {
                    category->setActivated();
                    sendForAllMembers(categoryID, returnMessage);
                    LOG(INFO) << "Category " << categoryID << " activated";
                } else {
                    auto member = category->findMember(senderID);
                    if (member->getStatus() == UNCONFIRMED) {
                        returnMessage->setServerInfoMessageType(FAIL);
                        returnMessage->setInfo("You are unconfirmed. You can't become online!");
                        controller->sendMessage(returnMessage, senderID);
                    } else {
                        member->setStatus(ONLINE);
                        controller->sendMessage(returnMessage, senderID);
                        LOG(INFO) << "User " << senderID << " deactivated";
                    }
                }
            } catch (out_of_range &exception) {
                LOG(DEBUG) << "Couldn't activate category " << categoryID << ". Couldn't find category";
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't find category");
                controller->sendMessage(returnMessage, senderID);
            } catch (exception &exception) {
                LOG(DEBUG) << "Couldn't activate category " << categoryID << ". Exception log: " << exception.what();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo(exception.what());
                controller->sendMessage(returnMessage, senderID);
            }
        }
            break;

        case MessageType::DEACTIVATE_CATEGORY: {
            long categoryID = categoryManagementMessage->getCategoryID();
            returnMessage->setExtraInfo(categoryID);

            try {
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                returnMessage->setServerInfoMessageType(CATEGORY_DEACTIVATED);
                if (senderID == owner->getID()) {
                    category->setActivated();
                    sendForAllMembers(categoryID, returnMessage);
                    LOG(INFO) << "Category " << categoryID << " deactivated";
                } else {
                    auto member = category->findMember(senderID);
                    if (member->getStatus() == UNCONFIRMED) {
                        returnMessage->setServerInfoMessageType(FAIL);
                        returnMessage->setInfo("You are unconfirmed. You can't become offline!");
                        controller->sendMessage(returnMessage, senderID);
                    } else {
                        member->setStatus(OFFLINE);
                        controller->sendMessage(returnMessage, senderID);
                        LOG(INFO) << "User " << senderID << " deactivated";
                    }
                }
            } catch (out_of_range &exception) {
                LOG(DEBUG) << "Couldn't deactivate category " << categoryID << ". Couldn't find category";
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't find category");
                controller->sendMessage(returnMessage, senderID);

            } catch (exception &exception) {
                LOG(DEBUG) << "Couldn't deactivate category " << categoryID << ". Exception log: " <<
                exception.what();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo(exception.what());
                controller->sendMessage(returnMessage, senderID);
            }
        }
            break;

        case NEW_MEMBER_CONFIRM: {
            long categoryID = categoryManagementMessage->getCategoryID();
            long memberID = categoryManagementMessage->getExtraInfo();
            returnMessage->setExtraInfo(categoryID);

            try {
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                if (owner->getID() != senderID) {
                    returnMessage->setServerInfoMessageType(FAIL);
                    returnMessage->setInfo("User is not an owner of category " + categoryID);
                    controller->sendMessage(returnMessage, senderID);
                } else {
                    auto member = category->findMember(memberID);
                    if (member->getStatus() != UNCONFIRMED) {
                        returnMessage->setServerInfoMessageType(FAIL);
                        returnMessage->setInfo("Member is already confirmed");
                        controller->sendMessage(returnMessage, senderID);
                    } else {
                        returnMessage->setServerInfoMessageType(OK);
                        returnMessage->setInfo("Activated user " + categoryID);
                        controller->sendMessage(returnMessage, senderID);

                        member->setStatus(ONLINE);

                        ServerInfoMessage *activatedMessage = new ServerInfoMessage();
                        activatedMessage->setType(SERVER_INFO);
                        activatedMessage->setServerInfoMessageType(MEMBER_CONFIRMED);
                        activatedMessage->setExtraInfo(categoryID);
                        controller->sendMessage(activatedMessage, memberID);

                        auto leftNeighbourID = member->getLeftNeighbour()->getUser()->getID();
                        auto rightNeighbourID = member->getRightNeighbour()->getUser()->getID();

                        sendNeighbours(categoryID, member->getUser()->getID());
                        sendNeighbours(categoryID, leftNeighbourID);
                        sendNeighbours(categoryID, rightNeighbourID);
                    }
                }

            } catch (out_of_range &exception) {
                LOG(DEBUG) << "Couldn't confirm new member. Couldn't find category " << categoryID;
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't find category");
                controller->sendMessage(returnMessage, senderID);

            } catch (exception &exception) {
                LOG(DEBUG) << "Couldn't confirm new member " << categoryID << ". Exception log: " <<
                exception.what();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo(exception.what());
                controller->sendMessage(returnMessage, senderID);
            }
        }
            break;

        case NEW_MEMBER_REJECT: {
            long categoryID = categoryManagementMessage->getCategoryID();
            long memberID = categoryManagementMessage->getExtraInfo();
            returnMessage->setExtraInfo(categoryID);

            try {
                auto category = model->getCategory(categoryID);
                auto owner = category->getOwner();
                if (owner->getID() != senderID) {
                    returnMessage->setServerInfoMessageType(FAIL);
                    returnMessage->setInfo("User is not an owner of category " + categoryID);
                    controller->sendMessage(returnMessage, senderID);
                } else {
                    auto member = category->findMember(memberID);
                    if (member->getStatus() != UNCONFIRMED) {
                        returnMessage->setServerInfoMessageType(FAIL);
                        returnMessage->setInfo("Member is already confirmed");
                        controller->sendMessage(returnMessage, senderID);
                    } else {
                        returnMessage->setServerInfoMessageType(OK);
                        returnMessage->setInfo("Refected user " + categoryID);
                        controller->sendMessage(returnMessage, senderID);

                        ServerInfoMessage *rejectedMessage = new ServerInfoMessage();
                        rejectedMessage->setType(SERVER_INFO);
                        rejectedMessage->setServerInfoMessageType(MEMBER_REJECTED);
                        rejectedMessage->setExtraInfo(categoryID);
                        controller->sendMessage(rejectedMessage, memberID);

                        category->removeMember(memberID);
                    }
                }

            } catch (out_of_range &exception) {
                LOG(DEBUG) << "Couldn't refect new member. Couldn't find category " << categoryID;
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo("Couldn't find category");
                controller->sendMessage(returnMessage, senderID);

            } catch (exception &exception) {
                LOG(DEBUG) << "Couldn't reject new member " << categoryID << ". Exception log: " <<
                exception.what();
                returnMessage->setServerInfoMessageType(FAIL);
                returnMessage->setInfo(exception.what());
                controller->sendMessage(returnMessage, senderID);
            }
        }
            break;

        default: {
            returnMessage->setServerInfoMessageType(FAIL);
            returnMessage->setInfo("Bad message type received");
            LOG(DEBUG) << "Received bad message type from user " << senderID;
            controller->sendMessage(returnMessage, senderID);
        }
            break;
    }
}

void CategoryManagementStrategy::sendNeighbours(long categoryID, long memberID) const {
    auto category = controller->getModel()->getCategory(categoryID);
    auto member = category->findMember(memberID);

    shared_ptr<CategoryMember> leftMember;
    shared_ptr<CategoryMember> rightMember;

    while ((leftMember = member->getLeftNeighbour())->getUser()->getID() != memberID ||
           leftMember->getStatus() == OFFLINE || rightMember->getStatus() == UNCONFIRMED);

    while ((rightMember = member->getRightNeighbour())->getUser()->getID() != memberID ||
           rightMember->getStatus() == OFFLINE || rightMember->getStatus() == UNCONFIRMED);

    auto leftNeighbour = leftMember->getUser();
    auto rightNeighbour = rightMember->getUser();

    NeighboursInfoMessage *infoMessage = new NeighboursInfoMessage(categoryID,
                                                                   leftNeighbour->getName(),
                                                                   leftNeighbour->getIP(),
                                                                   leftNeighbour->getPort(),
                                                                   rightNeighbour->getName(),
                                                                   rightNeighbour->getIP(),
                                                                   rightNeighbour->getPort());
    infoMessage->setType(NEIGHBOURS_SET);
    infoMessage->setSenderID(SERVER_ID);

    controller->sendMessage(infoMessage, memberID);
}

void CategoryManagementStrategy::sendForAllMembers(long categoryID, SimpleMessage *message) const {
    auto members = controller->getModel()->getCategory(categoryID)->getMembers();
    controller->sendMessage(message, members->getUser()->getID());

    for (auto categoryMember = members;
         categoryMember->getRightNeighbour() != members; categoryMember = categoryMember->getRightNeighbour()) {
        controller->sendMessage(message, categoryMember->getUser()->getID());
    }
}
