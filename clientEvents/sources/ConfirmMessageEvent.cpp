//
// Created by hubert.legec on 2015-12-15.
//

#include "../headers/ConfirmMessageEvent.h"

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

std::string ConfirmMessageEvent::toString() {
    //TODO
    return "ConfirmMessageEvent";
}