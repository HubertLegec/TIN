//
// Created by hubert on 05.12.15.
//

#ifndef RING_GETMESSAGE_H
#define RING_GETMESSAGE_H

#include "SimpleMessage.h"


class GetMessage : public SimpleMessage {
private:
    /*
     * specyfication of get request
     */
    GetMessageType requestType;
public:
    GetMessage(long senderID, GetMessageType request);
    GetMessage(char* data);

    GetMessageType getRequestType() const;
    void setRequestType(GetMessageType requestType);
};


#endif //RING_GETMESSAGE_H
