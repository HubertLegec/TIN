#include "../../../networkMessage/headers/CategoryManagementMessage.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../headers/CategoryManagementStrategy.h"
#include "../../model/headers/Model.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"
#include "../../utils/ServerGlobalConstants.h"

void CategoryManagementStrategy::serveEvent(SimpleMessage *message) const {
    CategoryManagementMessage *categoryManagementMessage = dynamic_cast<CategoryManagementMessage *> (message);

    long categoryID = categoryManagementMessage->getCategoryID();
    MessageType messageType = categoryManagementMessage->getMessageType();

    if (checkSender(message) == ServerGlobalConstant::FAILED_CODE)
        return;

    switch (messageType) {
        case MessageType::CREATE_CATEGORY: {
            auto categoryName = categoryManagementMessage->getCategoryName();
            createCategory(categoryName);
        }
            break;

        case MessageType::DESTROY_CATEGORY: {
            destroyCategory(categoryID);
        }
            break;

        case MessageType::JOIN_CATEGORY: {
            joinCategory(categoryID);
        }
            break;

        case MessageType::LEFT_CATEGORY: {
            leaveCategory(categoryID);
        }
            break;

        case MessageType::ACTIVATE_CATEGORY: {
            activateCategory(categoryID);
        }
            break;

        case MessageType::DEACTIVATE_CATEGORY: {
            deactivateCategory(categoryID);
        }
            break;

        case MessageType::NEW_MEMBER_CONFIRM: {
            long memberID = categoryManagementMessage->getExtraInfo();
            acceptMember(categoryID, memberID);
        }
            break;

        case MessageType::NEW_MEMBER_REJECT: {
            long memberID = categoryManagementMessage->getExtraInfo();
            rejectMember(categoryID, memberID);
        }
            break;

        default: {
            badMessageTypeReceived();
        }
            break;
    }
}

void CategoryManagementStrategy::createCategory(const string &categoryName) const {
    long categoryID = getModel()->createCategory(getSender(), categoryName);

    if (categoryID == ServerGlobalConstant::FAILED_CODE) {
        sendMessage(getSender(), ServerGlobalConstant::FAILED_CODE, ServerInfoMessageType::FAIL,
                    "Failed to create category " + categoryName);
    } else {
        LOG(INFO) << "Created category " << categoryName;
        sendMessage(getSender(), categoryID, ServerInfoMessageType::CATEGORY_CREATED, categoryName);
        auto category = getModel()->getCategory(categoryID);
        sendNeighbours(category, category->getMembers());
    }
}

void CategoryManagementStrategy::destroyCategory(long categoryID) const {
    auto category = getModel()->getCategory(categoryID);
    if (!category) {
        sendCategoryNotFound(getSender(), categoryID);
        return;
    }
    auto owner = category->getOwner();
    if (owner->getID() != getSender()->getID()) {
        LOG(INFO) << "Failed to destroy category. User " << getSender()->getID() << " is not an owner of category " <<
        categoryID;
        sendMessage(getSender(), categoryID, ServerInfoMessageType::FAIL, "User not allowed to destroy category.");
    } else {
        sendForAllMembers(category, categoryID, ServerInfoMessageType::CATEGORY_REMOVED);
        getModel()->deleteCategory(category);
        LOG(INFO) << "Destroyed category " << categoryID;
    }
}

void CategoryManagementStrategy::joinCategory(long categoryID) const {
    auto category = getModel()->getCategory(categoryID);
    if (!category) {
        sendCategoryNotFound(getSender(), categoryID);
        return;
    }
    auto owner = category->getOwner();
    category->addNewMember(getSender());
    LOG(INFO) << "Added user " << getSender()->getName() << " to category " << categoryID;
    sendMessage(getSender(), categoryID, ServerInfoMessageType::CATEGORY_JOINED, category->getName());

    //client needs userID, so it is send as getSender() id instead of SERVER_ID
    ServerInfoMessage *newMemberMessage = new ServerInfoMessage(getSender()->getID(),
                                                                ServerInfoMessageType::NEW_CATEGORY_MEMBER,
                                                                getSender()->getName() + " " +
                                                                getSender()->getIP());
    newMemberMessage->setExtraInfo(categoryID);
    controller->sendMessage(shared_ptr<ServerInfoMessage>(newMemberMessage), owner);
}

void CategoryManagementStrategy::leaveCategory(long categoryID) const {
    auto category = getModel()->getCategory(categoryID);
    if (!category) {
        sendCategoryNotFound(getSender(), categoryID);
        return;
    }

    auto memberToRemove = category->findMember(getSender()->getID());
    if (!memberToRemove) {
        sendMemberNotFound(getSender(), categoryID, getSender()->getID());
        return;
    }

    auto leftNeighbour = memberToRemove->getLeftNeighbour();
    auto rightNeighbour = memberToRemove->getRightNeighbour();

    category->removeMember(getSender()->getID());
    LOG(INFO) << "User " << getSender()->getID() << " left category " << categoryID;

    if (leftNeighbour != memberToRemove)
        sendNeighbours(category, leftNeighbour);
    if (rightNeighbour != memberToRemove)
        sendNeighbours(category, rightNeighbour);

    sendMessage(getSender(), categoryID, ServerInfoMessageType::CATEGORY_LEFT);
}

void CategoryManagementStrategy::activateCategory(long categoryID) const {
    auto category = getModel()->getCategory(categoryID);
    if (!category) {
        sendCategoryNotFound(getSender(), categoryID);
        return;
    }
    auto owner = category->getOwner();

    if (getSender()->getID() == owner->getID()) {
        category->setActivated();
        sendForAllMembers(category, categoryID, ServerInfoMessageType::CATEGORY_DEACTIVATED);
        LOG(INFO) << "Category " << categoryID << " activated";
    } else if (category->isUnconfirmed(getSender()->getID())) {
        sendMessage(getSender(), categoryID, FAIL, "You are unconfirmed. You can't become online!");
    } else {
        auto member = category->findMember(getSender()->getID());
        if (!member) {
            sendMemberNotFound(getSender(), categoryID, getSender()->getID());
            return;
        }
        member->setStatus(Status::ONLINE);
        sendMessage(getSender(), categoryID, ServerInfoMessageType::CATEGORY_ACTIVATED);
        sendAllNeighbours(category, member);
    }
}

void CategoryManagementStrategy::deactivateCategory(long categoryID) const {
    auto category = getModel()->getCategory(categoryID);
    if (!category) {
        sendCategoryNotFound(getSender(), categoryID);
        return;
    }
    auto owner = category->getOwner();

    if (getSender()->getID() == owner->getID()) {
        category->setDeactivated();
        sendForAllMembers(category, categoryID, ServerInfoMessageType::CATEGORY_DEACTIVATED);
        LOG(INFO) << "Category " << categoryID << " deactivated";
    } else if (category->isUnconfirmed(getSender()->getID())) {
        sendMessage(getSender(), categoryID, ServerInfoMessageType::FAIL,
                    "You are unconfirmed. You can't become offline!");
    } else {
        auto member = category->findMember(getSender()->getID());
        member->setStatus(Status::OFFLINE);
        sendMessage(getSender(), categoryID, ServerInfoMessageType::CATEGORY_DEACTIVATED);
        sendAllNeighbours(category, member);
    }
}

void CategoryManagementStrategy::acceptMember(long categoryID, long memberID) const {
    auto category = getModel()->getCategory(categoryID);
    if (!category) {
        sendCategoryNotFound(getSender(), categoryID);
        return;
    }
    if (!getModel()->getUser(memberID)) {
        sendUserNotFound(getSender(), memberID);
        return;
    }
    auto owner = category->getOwner();
    if (owner->getID() != getSender()->getID()) {
        sendMessage(getSender(), categoryID, ServerInfoMessageType::FAIL,
                    "User is not an owner of category " + categoryID);
    } else {
        if (category->isUnconfirmed(memberID)) {
            sendMessage(getSender(), categoryID, ServerInfoMessageType::OK, "Activated user in category" + categoryID);
            category->acceptNewUser(memberID);
            auto member = category->findMember(memberID);
            sendMessage(member->getUser(), categoryID, ServerInfoMessageType::MEMBER_CONFIRMED);
            sendAllNeighbours(category, member);

        } else {
            sendMessage(getSender(), categoryID, ServerInfoMessageType::FAIL,
                        "Member is already confirmed or didn't join the category");
        }
    }
}

void CategoryManagementStrategy::rejectMember(long categoryID, long memberID) const {
    auto category = getModel()->getCategory(categoryID);
    if (!category) {
        sendCategoryNotFound(getSender(), categoryID);
        return;
    }
    auto owner = category->getOwner();
    auto rejectedUser = getModel()->getUser(memberID);
    if (!rejectedUser) {
        sendUserNotFound(getSender(), memberID);
        return;
    }

    if (owner->getID() != getSender()->getID()) {
        sendMessage(getSender(), categoryID, ServerInfoMessageType::FAIL,
                    "User is not an owner of category " + categoryID);
    } else {
        if (category->isUnconfirmed(memberID)) {
            category->rejectMember(memberID);
            sendMessage(getSender(), categoryID, ServerInfoMessageType::OK, "Rejected user " + categoryID);
            sendMessage(rejectedUser, categoryID, ServerInfoMessageType::MEMBER_REJECTED,
                        "Rejected user in category " + categoryID);
        } else {
            sendMessage(getSender(), categoryID, ServerInfoMessageType::FAIL,
                        "Member is already confirmed or didn't join the category");
        }
    }
}
