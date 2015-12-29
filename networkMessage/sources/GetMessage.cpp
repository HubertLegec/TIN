//
// Created by hubert on 05.12.15.
//

#include "../headers/GetMessage.h"

GetMessage::GetMessage() { }

GetMessage::GetMessage(long senderID, GetMessageType request) : SimpleMessage(MessageType::GET, senderID),
                                                                requestType(request) {
    categoryID = UNDEFINED_ID;
    this->size = SimpleMessage::getMessageSize() + sizeof(requestType) + sizeof(categoryID);
}

void GetMessage::setRequestType(GetMessageType requestType) {
    this->requestType = requestType;
}

GetMessageType GetMessage::getRequestType() const {
    return requestType;
}

void GetMessage::setCategoryID(long categoryID) {
    this->categoryID = categoryID;
}

long GetMessage::getCategoryID() const {
    return categoryID;
}

std::string GetMessage::toString() {
    std::stringstream ss;
    ss << "GetMessage[type:" << type << "; ";
    ss << "size:" << size << "; ";
    ss << "senderID:" << senderID << "; ";
    ss << "requestType:" << requestType << "; ";
    ss << "categoryID:" << categoryID << "]";
    return ss.str();
}