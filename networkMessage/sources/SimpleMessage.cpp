//
// Created by hubert on 05.12.15.
//

#include "../headers/SimpleMessage.h"


SimpleMessage::SimpleMessage(MessageType type, long senderID) : type(type), senderID(senderID) {
    this->size = sizeof(type) + sizeof(senderID) + sizeof(size);
}

SimpleMessage::SimpleMessage(char *data) {
    //TODO
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