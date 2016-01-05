//
// Created by hubert on 05.12.15.
//

#ifndef RING_GETMESSAGE_H
#define RING_GETMESSAGE_H

#include "SimpleMessage.h"
#include "../../cereal/archives/binary.hpp"

/**
 * This class is used by client to send to server some request
 */
class GetMessage : public SimpleMessage {
private:
    /*
     * specyfication of get request
     */
    GetMessageType requestType;
    long categoryID;
public:
    static const long UNDEFINED_ID = -1;

    GetMessage();

    GetMessage(long senderID, GetMessageType request);

    GetMessage(long senderID, GetMessageType request, long categoryID);

    GetMessageType getRequestType() const;

    void setRequestType(GetMessageType requestType);

    long getCategoryID() const;

    void setCategoryID(long categoryID);

    virtual std::string toString() const;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(this->type, this->size, this->senderID, this->requestType, this->categoryID);
    }
};


#endif //RING_GETMESSAGE_H
