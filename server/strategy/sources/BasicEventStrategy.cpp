#include "../headers/BasicEventStrategy.h"
#include "../../controller/headers/Controller.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"
#include "../../../logger/easylogging++.h"
#include "../../utils/ServerGlobalConstants.h"

BasicEventStrategy::BasicEventStrategy() : controller(nullptr) { }

BasicEventStrategy::BasicEventStrategy(Controller *controller) : controller(controller) { }

shared_ptr<Model> BasicEventStrategy::getModel() const {
    return controller->getModel();
}

void BasicEventStrategy::setController(Controller *controller) {
    this->controller = controller;
}

long BasicEventStrategy::checkSender(SimpleMessage *message) const {
    auto senderID = message->getSenderID();
    sender = getModel()->getUser(senderID);
    if (!sender) {
        LOG(INFO) << "Couldn't find user who sent message. SenderID: " << senderID;
        return ServerGlobalConstant::FAILED_CODE;
    }

    return senderID;
}

void BasicEventStrategy::sendMessage(shared_ptr<User> user, ServerInfoMessageType infoMessageType) const {
    sendMessage(user, ServerGlobalConstant::UNKNOWN_CODE, infoMessageType, "");
}

void BasicEventStrategy::sendMessage(shared_ptr<User> user, ServerInfoMessageType infoMessageType,
                                     const string &info) const {
    sendMessage(user, ServerGlobalConstant::UNKNOWN_CODE, infoMessageType, info);
}

void BasicEventStrategy::sendMessage(shared_ptr<User> user, long extraInfo,
                                     ServerInfoMessageType infoMessageType) const {
    sendMessage(user, extraInfo, infoMessageType, "");
}

void BasicEventStrategy::sendMessage(shared_ptr<User> user, long extraInfo, ServerInfoMessageType infoMessageType,
                                     const string &info) const {
    ServerInfoMessage *message = new ServerInfoMessage(ServerGlobalConstant::SERVER_ID, infoMessageType, info);
    message->setExtraInfo(extraInfo);
    controller->sendMessage(shared_ptr<ServerInfoMessage>(message), user);
}

void BasicEventStrategy::sendNeighbours(shared_ptr<Category> category, shared_ptr<CategoryMember> member) const {
    auto leftMember = member->getLeftNeighbour();
    auto rightMember = member->getRightNeighbour();

    while (leftMember != member && leftMember->getStatus() == Status::OFFLINE) {
        leftMember = leftMember->getLeftNeighbour();
    }

    while (rightMember != member && rightMember->getStatus() == Status::OFFLINE) {
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
    infoMessage->setType(MessageType::NEIGHBOURS_SET);
    infoMessage->setSenderID(ServerGlobalConstant::SERVER_ID);

    controller->sendMessage(shared_ptr<NeighboursInfoMessage>(infoMessage), member->getUser());
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

void BasicEventStrategy::sendForAllMembers(shared_ptr<Category> category, shared_ptr<SimpleMessage> message) const {
    auto members = category->getMembers();
    auto categoryMember = members;
    do {
        controller->sendMessage(message, categoryMember->getUser());
    } while ((categoryMember = categoryMember->getLeftNeighbour()) != members);
}

void BasicEventStrategy::sendForAllMembers(shared_ptr<Category> category, long extraInfo,
                                           ServerInfoMessageType infoMessageType) const {
    shared_ptr<ServerInfoMessage> message(new ServerInfoMessage(ServerGlobalConstant::SERVER_ID, infoMessageType, ""));
    message->setExtraInfo(extraInfo);
    sendForAllMembers(category, message);
}

void BasicEventStrategy::sendCategoryNotFound(shared_ptr<User> sender, long categoryID) const {
    sendMessage(sender, categoryID, ServerInfoMessageType::FAIL, "Couldn't find category" + categoryID);
}

void BasicEventStrategy::sendMemberNotFound(shared_ptr<User> sender, long categoryID, long userID) const {
    sendMessage(sender, categoryID, ServerInfoMessageType::FAIL, "Couldn't find member" + userID);
}

void BasicEventStrategy::sendUserNotFound(shared_ptr<User> sender, long userID) const {
    sendMessage(sender, userID, ServerInfoMessageType::FAIL, "Couldn't find user" + userID);
}

shared_ptr<User> BasicEventStrategy::getSender() const {
    return sender;
}

void BasicEventStrategy::badMessageTypeReceived() const {
    sendMessage(getSender(), ServerInfoMessageType::FAIL, "Bad message type received");
    LOG(INFO) << "Received bad message type from user " << getSender()->getID();
}

void BasicEventStrategy::badMessageTypeReceived(const string &senderIP, int senderPort) const {
    ServerInfoMessage *returnMessage = new ServerInfoMessage(ServerGlobalConstant::SERVER_ID,
                                                             ServerInfoMessageType::FAIL,
                                                             "Bad message type received!");
    controller->sendMessage(returnMessage, senderIP, senderPort);
    LOG(INFO) << "Received bad message type from ip " << senderIP;
}
