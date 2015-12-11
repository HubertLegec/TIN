//
// Created by hubert on 05.12.15.
//

#include <sstream>
#include "../headers/SimpleMessage.h"
#include "../../cereal/access.hpp"

SimpleMessage::SimpleMessage() { }

SimpleMessage::SimpleMessage(MessageType type, long senderID) : type(type), senderID(senderID) {
    this->size = sizeof(type) + sizeof(senderID) + sizeof(size);
}

SimpleMessage::SimpleMessage(char* data) {
    SimpleMessage* tmp = (SimpleMessage*)data;
    this->type = tmp->type;
    this->senderID - tmp->senderID;
    this->size = tmp->size;
}

int SimpleMessage::getMessageSize() const {
    return size;
}

MessageType SimpleMessage::getMessageType() const {
    return type;
}

long SimpleMessage::getSenderID() const {
    return senderID;
}

void SimpleMessage::setSenderID(long id) {
    this->senderID = senderID;
}

void SimpleMessage::setType(MessageType type) {
    this->type = type;
}