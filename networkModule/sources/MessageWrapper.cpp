//
// Created by hubert on 28.12.15.
//

#include "../headers/MessageWrapper.h"

MessageWrapper::MessageWrapper(std::shared_ptr<SimpleMessage> message, std::string ip, int port) {
    this->message = message;
    this->ip = ip;
    this->port = port;
}

std::string MessageWrapper::getIP() const {
    return ip;
}

int MessageWrapper::getPort() const {
    return port;
}

std::shared_ptr<SimpleMessage> MessageWrapper::getMessage() const {
    return message;
}