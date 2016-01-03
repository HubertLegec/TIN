//
// Created by hubert on 03.01.16.
//

#include "../headers/NetworkControllerErrorMessage.h"

using namespace std;

NetworkControllerErrorMessage::NetworkControllerErrorMessage()
        : SimpleMessage(MessageType::NETWORK_CONTROLLER_ERROR_MESSAGE, SimpleMessage::UNDEFINED_ID) {
    this->size = SimpleMessage::getMessageSize() + sizeof(ErrorCode) + sizeof(long);
}

NetworkControllerErrorMessage::NetworkControllerErrorMessage(ErrorCode code, const std::string &info)
        : SimpleMessage(MessageType::NETWORK_CONTROLLER_ERROR_MESSAGE, SimpleMessage::UNDEFINED_ID) {
    this->size = SimpleMessage::getMessageSize() + sizeof(ErrorCode) + sizeof(long) + info.size();
    this->info = info;
    this->code = code;
}

void NetworkControllerErrorMessage::setInfo(const std::string &info) {
    this->size -= this->info.size();
    this->info = info;
    this->size += info.size();
}

void NetworkControllerErrorMessage::setErrorCode(ErrorCode code) {
    this->code = code;
}

ErrorCode NetworkControllerErrorMessage::getErrorCode() const {
    return code;
}

const std::string &NetworkControllerErrorMessage::getInfo() const {
    return info;
}

std::string NetworkControllerErrorMessage::toString() const {
    stringstream ss;
    ss << "NetworkControllerErrorMessage[type:" << type << "; ";
    ss << "size:" << size << "; ";
    ss << "senderID:" << senderID << "; ";
    ss << "errorCode:" << code << "; ";
    ss << "info:" << info << "]";
    return ss.str();
}