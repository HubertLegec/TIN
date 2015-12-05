//
// Created by hubert on 05.12.15.
//

#include "GetMessage.h"


GetMessage::GetMessage(long senderID, GetMessageType request) : SimpleMessage(MessageType::GET, senderID),
                                                                requestType(request) {
    this->size = SimpleMessage::getMessageSize() + sizeof(requestType);
}

void GetMessage::setRequestType(GetMessageType requestType) {
    this->requestType = requestType;
}

GetMessageType GetMessage::getRequestType() const {
    return requestType;
}

