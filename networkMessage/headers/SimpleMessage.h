//
// Created by hubert on 05.12.15.
//

#ifndef RING_SIMPLEMESSAGE_H
#define RING_SIMPLEMESSAGE_H

#include "MessageTypes.h"
#include <string>
#include "../../cereal/archives/binary.hpp"

/*
 * Simple message class
 * All others network message classes should inherit from this class
 */
class SimpleMessage {
private:
    /*
     * All the message types should be defined in MessageType enum
     */
    MessageType type;
protected:
    /* message size in bytes
     * it should be automatically calculated when we set message content
     */
    int size;
private:
    /*
     * message sender id
     * server id should be set to 0
     * others' id should be given by server
     */
    long senderID;

    friend class cereal::access;
public:
    SimpleMessage();
    SimpleMessage(MessageType type, long senderID);
    SimpleMessage(char* data);

    void setType(MessageType type);
    MessageType getMessageType() const;
    void setSenderID(long id);
    long getSenderID() const;
    int getMessageSize() const;
    template<class Archive>
    void serialize(Archive & archive){
        archive(this->type, this->senderID, this->size);
    }
};


#endif //RING_SIMPLEMESSAGE_H
