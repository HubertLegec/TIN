//
// Created by hubert on 05.12.15.
//

#ifndef RING_GETMESSAGE_H
#define RING_GETMESSAGE_H

#include "SimpleMessage.h"
#include "../../cereal/archives/binary.hpp"


class GetMessage : public SimpleMessage {
private:
    /*
     * specyfication of get request
     */
    GetMessageType requestType;
    long categoryID;
public:
    GetMessage();
    GetMessage(long senderID, GetMessageType request);

    GetMessageType getRequestType() const;
    void setRequestType(GetMessageType requestType);

    long getCategoryID() const;

    void setCategoryID(long categoryID);
    std::string toString();
    template<class Archive>
    void serialize(Archive & archive){
        archive(this->type, this->size, this->senderID, this->requestType, this->categoryID);
    }
};


#endif //RING_GETMESSAGE_H
