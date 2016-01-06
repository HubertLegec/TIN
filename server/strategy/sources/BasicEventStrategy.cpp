#include "../headers/BasicEventStrategy.h"
#include "../../controller/headers/Controller.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"

BasicEventStrategy::BasicEventStrategy() : controller(nullptr) { }

BasicEventStrategy::BasicEventStrategy(Controller *controller) : controller(controller) { }

void BasicEventStrategy::setController(Controller *controller) {
    this->controller = controller;
}

void BasicEventStrategy::sendMessage(shared_ptr<User> user, ServerInfoMessageType infoMessageType) const {
    sendMessage(user, UNKNOWN_CODE, infoMessageType, "");
}

void BasicEventStrategy::sendMessage(shared_ptr<User> user, ServerInfoMessageType infoMessageType,
                                     const string &info) const {
    sendMessage(user, UNKNOWN_CODE, infoMessageType, info);
}

void BasicEventStrategy::sendMessage(shared_ptr<User> user, long extraInfo,
                                     ServerInfoMessageType infoMessageType) const {
    sendMessage(user, extraInfo, infoMessageType, "");
}

void BasicEventStrategy::sendMessage(shared_ptr<User> user, long extraInfo, ServerInfoMessageType infoMessageType,
                                     const string &info) const {
    ServerInfoMessage *message = new ServerInfoMessage(SERVER_ID, infoMessageType, info);
    message->setExtraInfo(extraInfo);
    controller->sendMessage(message, user);
}

void BasicEventStrategy::sendNeighbours(shared_ptr<Category> category, shared_ptr<CategoryMember> member) const {
    auto leftMember = member->getLeftNeighbour();
    auto rightMember = member->getRightNeighbour();

    while (leftMember != member && leftMember->getStatus() == OFFLINE) {
        leftMember = leftMember->getLeftNeighbour();
    }

    while (rightMember != member && rightMember->getStatus() == OFFLINE) {
        rightMember = rightMember->getLeftNeighbour();
    }

    auto leftNeighbour = leftMember->getUser();
    auto rightNeighbour = rightMember->getUser();

    NeighboursInfoMessage *infoMessage = new NeighboursInfoMessage(category->getID(),
                                                                   leftNeighbour->getName(),
                                                                   leftNeighbour->getIP(),
                                                                   leftNeighbour->getPort(),
                                                                   rightNeighbour->getName(),
                                                                   rightNeighbour->getIP(),
                                                                   rightNeighbour->getPort());
    infoMessage->setType(NEIGHBOURS_SET);
    infoMessage->setSenderID(SERVER_ID);

    controller->sendMessage(infoMessage, member->getUser());
}

void BasicEventStrategy::sendAllNeighbours(shared_ptr<Category> category, shared_ptr<CategoryMember> member) const {
    auto leftNeighbour = member->getLeftNeighbour();
    auto rightNeighbour = member->getRightNeighbour();

    sendNeighbours(category, member);
    if (leftNeighbour != member)
        sendNeighbours(category, leftNeighbour);
    if (rightNeighbour != member && rightNeighbour != leftNeighbour)
        sendNeighbours(category, rightNeighbour);
}

void BasicEventStrategy::sendForAllMembers(shared_ptr<Category> category, SimpleMessage *message) const {
    auto members = category->getMembers();
    auto categoryMember = members;
    do {
        controller->sendMessage(message, categoryMember->getUser());
    } while ((categoryMember = categoryMember->getLeftNeighbour()) != members);
}

void BasicEventStrategy::sendForAllMembers(shared_ptr<Category> category, long extraInfo,
                                           ServerInfoMessageType infoMessageType) const {
    ServerInfoMessage *message = new ServerInfoMessage(SERVER_ID, infoMessageType, "");
    message->setExtraInfo(extraInfo);
    sendForAllMembers(category, message);
}

void BasicEventStrategy::sendCategoryNotFound(shared_ptr<User> sender, long categoryID) const {
    sendMessage(sender, categoryID, FAIL, "Couldn't find category" + categoryID);
}

void BasicEventStrategy::sendMemberNotFound(shared_ptr<User> sender, long categoryID, long userID) const {
    sendMessage(sender, categoryID, FAIL, "Couldn't find member" + userID);
}

void BasicEventStrategy::sendUserNotFound(shared_ptr<User> sender, long userID) const {
    sendMessage(sender, userID, FAIL, "Couldn't find user" + userID);
}
