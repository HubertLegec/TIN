//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/ServerInfoMessage.h"


ServerInfoMessage::ServerInfoMessage() : SimpleMessage() {

}

ServerInfoMessage::ServerInfoMessage(long senderID, ServerInfoMessageType infoType, const std::string& info)
                                    : SimpleMessage(MessageType::SERVER_INFO, senderID), infoType(infoType), info(info){
    this->extraInfo = -1;
    std::cout << "SM size: " << SimpleMessage::getMessageSize() << std::endl;
    std::cout << "info size: " << info.size() << std::endl;
    this->size = SimpleMessage::getMessageSize() + sizeof(infoType) +2*sizeof(extraInfo) + info.size();
    std::cout << "result size: " << this->size << std::endl;
}

ServerInfoMessageType ServerInfoMessage::getInfoType() const {
    return infoType;
}

std::string ServerInfoMessage::getInfo() const {
        return std::string(info);
}

void ServerInfoMessage::setInfo(const std::string info) {
    this->info = info;
}

void ServerInfoMessage::setExtraInfo(long extraInfo) {
    this->extraInfo = extraInfo;
}

void ServerInfoMessage::setServerInfoMessageType(ServerInfoMessageType infoType) {
    this->infoType = infoType;

}

long ServerInfoMessage::getExtraInfo() const {
    return extraInfo;
}

std::string ServerInfoMessage::toString() {
    //TODO
    return "ServerInfoMessage";
}