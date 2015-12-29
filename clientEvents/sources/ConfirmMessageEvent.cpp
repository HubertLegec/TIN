//
// Created by hubert.legec on 2015-12-15.
//

#include <sstream>
#include "../headers/ConfirmMessageEvent.h"

using namespace std;

ConfirmMessageEvent::ConfirmMessageEvent() : BasicEvent() { }

ConfirmMessageEvent::ConfirmMessageEvent(long messageIndex) {
    this->messageIndex = messageIndex;
}

long ConfirmMessageEvent::getMessageIndex() const {
    return messageIndex;
}

void ConfirmMessageEvent::setMessageIndex(long messageIndex) {
    this->messageIndex = messageIndex;
}

string ConfirmMessageEvent::toString() const {
    stringstream ss;
    ss << "ConfirmMessageEvent[messageIndex:" << messageIndex << "]";
    return ss.str();
}

string ConfirmMessageEvent::getName() const {
    return "CONFIRM_MESSAGE_EVENT";
}