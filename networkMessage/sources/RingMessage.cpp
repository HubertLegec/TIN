//
// Created by hubert on 06.12.15.
//

#include <cstring>
#include "../headers/RingMessage.h"

using namespace std;

RingMessage::RingMessage() : SimpleMessage() {
    this->size = SimpleMessage::getMessageSize() + 3 * sizeof(long);
}

RingMessage::RingMessage(long senderID, long categoryId, const string &text) : SimpleMessage(MessageType::RING_MESSAGE,
                                                                                             senderID) {
    this->messageText = text;
    this->categoryId = categoryId;
    this->size = SimpleMessage::getMessageSize() + 3 * sizeof(long) + text.size();
}

RingMessage::RingMessage(long senderID, long categoryId, const string &text, const vector<string> &confirmations)
        : SimpleMessage(MessageType::RING_MESSAGE, senderID), confirmations(confirmations) {
    this->messageText = text;
    this->categoryId = categoryId;
    this->size = SimpleMessage::getMessageSize() + 3 * sizeof(long) + text.size();
    for (string s : confirmations) {
        size += (sizeof(long) + s.size());
    }
}

RingMessage::RingMessage(const RingMessage &other) : SimpleMessage(other) {
    this->messageText = other.messageText;
    this->categoryId = other.categoryId;
    this->confirmations = other.confirmations;
}

RingMessage &RingMessage::operator=(const RingMessage &other) {
    SimpleMessage::operator=(other);
    this->messageText = other.messageText;
    this->categoryId = other.categoryId;
    this->confirmations = other.confirmations;
    return *this;
}

void RingMessage::addConfirmation(const string &name) {
    confirmations.push_back(name);
    size += (sizeof(long) + name.size());
}

void RingMessage::addConfirmations(const vector<string> &nameList) {
    for (string s : nameList) {
        addConfirmation(s);
    }
}

vector<string> RingMessage::getConfirmationsList() const {
    return confirmations;
}

const string &RingMessage::getMsgText() const {
    return messageText;
}

long RingMessage::getCategoryId() const {
    return categoryId;
}

string RingMessage::toString() const {
    stringstream ss;
    ss << "RingMessage[type:" << type << "; ";
    ss << "size:" << size << "; ";
    ss << "senderID:" << senderID << ";\n";
    ss << "categoryID:" << categoryId << "; ";
    ss << "messageText:" << messageText << "; ";
    ss << "confirmations:{";
    for (string confirmation : confirmations) {
        ss << endl << confirmation;
    }
    ss << "}]";
    return ss.str();
}