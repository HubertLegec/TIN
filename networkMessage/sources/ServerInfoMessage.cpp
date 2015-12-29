//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/ServerInfoMessage.h"

using namespace std;

ServerInfoMessage::ServerInfoMessage() : SimpleMessage() {

}

ServerInfoMessage::ServerInfoMessage(long senderID, ServerInfoMessageType infoType, const std::string& info)
                                    : SimpleMessage(MessageType::SERVER_INFO, senderID), infoType(infoType), info(info){
    this->extraInfo = -1;
    this->size = SimpleMessage::getMessageSize() + sizeof(infoType) +2*sizeof(extraInfo) + info.size();
}

ServerInfoMessageType ServerInfoMessage::getInfoType() const {
    return infoType;
}

const string &ServerInfoMessage::getInfo() const {
    return info;
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

string ServerInfoMessage::toString() const {
    stringstream ss;
    ss << "ServerInfoMessage[type:" << type << "; ";
    ss << "size:" << size << "; ";
    ss << "senderID:" << senderID << ";\n";
    ss << "infoType:" << infoType << "; ";
    ss << "info:" << info << "; ";
    ss << "extraInfo:" << extraInfo << "]";
    return ss.str();
}