#include "../headers/BasicEventStrategy.h"
#include "../../controller/headers/Controller.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"

BasicEventStrategy::BasicEventStrategy() : controller(nullptr) { }

BasicEventStrategy::BasicEventStrategy(Controller *controller) : controller(controller) { }

void BasicEventStrategy::setController(Controller *controller) {
    this->controller = controller;
}

void BasicEventStrategy::sendMessage(long userID, ServerInfoMessageType infoMessageType) const {
    sendMessage(userID, -1, infoMessageType, "");
}

void BasicEventStrategy::sendMessage(long userID, ServerInfoMessageType infoMessageType, const string& info) const {
    sendMessage(userID, -1, infoMessageType, info);
}

void BasicEventStrategy::sendMessage(long userID, long extraInfo, ServerInfoMessageType infoMessageType) const {
    sendMessage(userID, extraInfo, infoMessageType, "");
}

void BasicEventStrategy::sendMessage(long userID, long extraInfo, ServerInfoMessageType infoMessageType,
                                     const string &info) const {
    ServerInfoMessage *message = new ServerInfoMessage(SERVER_ID, infoMessageType, info);
    message->setExtraInfo(extraInfo);
    controller->sendMessage(message, userID);
}

void BasicEventStrategy::sendNeighbours(long categoryID, shared_ptr<CategoryMember> member) const {
    shared_ptr<CategoryMember> leftMember;
    shared_ptr<CategoryMember> rightMember;

    while (*(leftMember = member->getLeftNeighbour()) != *member ||
           leftMember->getStatus() == OFFLINE);

    while (*(rightMember = member->getRightNeighbour()) != *member ||
           rightMember->getStatus() == OFFLINE);

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

    controller->sendMessage(infoMessage, member->getUser());
}

void BasicEventStrategy::sendNeighbours(long categoryID, long memberID) const {
    auto category = controller->getModel()->getCategory(categoryID);
    auto member = category->findMember(memberID);
    sendNeighbours(categoryID, member);
}


void BasicEventStrategy::sendAllNeighbours(long categoryID, long memberID) const {
    auto category = controller->getModel()->getCategory(categoryID);
    auto member = category->findMember(memberID);
    auto leftNeighbour = member->getLeftNeighbour();
    auto rightNeighbour = member->getRightNeighbour();

    sendNeighbours(categoryID, member);
    if (*leftNeighbour != *member)
        sendNeighbours(categoryID, leftNeighbour);
    if (*rightNeighbour != *member)
        sendNeighbours(categoryID, rightNeighbour);
}

void BasicEventStrategy::sendForAllMembers(long categoryID, SimpleMessage *message) const {
    auto members = controller->getModel()->getCategory(categoryID)->getMembers();
    auto categoryMember = members;
    do {
        controller->sendMessage(message, categoryMember->getUser());
    } while ((categoryMember = categoryMember->getLeftNeighbour()) != members);
}
