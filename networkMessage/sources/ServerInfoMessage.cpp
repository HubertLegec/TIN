//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/ServerInfoMessage.h"

ServerInfoMessage::ServerInfoMessage(long senderID, ServerInfoMessageType infoType, const std::string& info)
                                    : SimpleMessage(MessageType::SERVER_INFO, senderID), infoType(infoType){
    this->infoSize = info.size();
        this->info = new char[infoSize + 1];
        strcpy(this->info, info.c_str());

    this->extraInfo = -1;
    this->size = SimpleMessage::getMessageSize() + sizeof(infoSize) + sizeof(extraInfo) + infoSize+1;
}

ServerInfoMessage::ServerInfoMessage(char *data) : SimpleMessage(data) {
    //TODO
}

ServerInfoMessage::~ServerInfoMessage() {
    delete[] info;
}

ServerInfoMessageType ServerInfoMessage::getInfoType() const {
    return infoType;
}

std::string ServerInfoMessage::getInfo() const {
        return std::string(info);
}

void ServerInfoMessage::setInfo(const std::string info) {
    if(this->info != nullptr){
        delete[] this->info;
    }

    this->infoSize = info.size();
        this->info = new char[infoSize + 1];
        strcpy(this->info, info.c_str());
}

void ServerInfoMessage::setExtraInfo(long extraInfo) {
    this->extraInfo = extraInfo;
}

long ServerInfoMessage::getExtraInfo() const {
    return extraInfo;
}