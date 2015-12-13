//
// Created by hubert on 06.12.15.
//

#include <cstring>
#include "../headers/RingMessage.h"

RingMessage::RingMessage() : SimpleMessage() { }

RingMessage::RingMessage(long senderID, const std::string &text) : SimpleMessage(MessageType::RING_MESSAGE, senderID) {
    this->messageText = text;

    this->size = SimpleMessage::getMessageSize() + 2*sizeof(long) + text.size();
}

RingMessage::RingMessage(long senderID, const std::string &text, const std::vector<std::string> &confirmations)
                        : SimpleMessage(MessageType::RING_MESSAGE, senderID), confirmations(confirmations){
    this->size = SimpleMessage::getMessageSize() + 2*sizeof(long) + text.size();
    for(std::string s : confirmations){
        size += (sizeof(long) + s.size());
    }
}

RingMessage::RingMessage(const RingMessage &other) : SimpleMessage(other) {
    this->messageText = other.messageText;
    this->confirmations = other.confirmations;
}

RingMessage& RingMessage::operator=(const RingMessage &other){
    SimpleMessage::operator=(other);
    this->messageText = other.messageText;
    this->confirmations = other.confirmations;
    return *this;
}

void RingMessage::addConfirmation(const std::string &name) {
    confirmations.push_back(name);
    size += (sizeof(long) + name.size());
}

void RingMessage::addConfirmations(const std::vector<std::string> &nameList) {
    for(std::string s : nameList){
        addConfirmation(s);
    }
}

std::vector<std::string> RingMessage::getConfirmationsList() const {
    return confirmations;
}

std::string RingMessage::getMsgText() const {
    return messageText;
}


