//
// Created by hubert on 05.12.15.
//

#ifndef RING_SIMPLEMESSAGE_H
#define RING_SIMPLEMESSAGE_H

#include "MessageType.h"

/*
 * Simple message class
 * All others network message classes should inherit from this class
 */
virtual class SimpleMessage {
private:
    /*
     * All the message types should be defined in MessageType enum
     */
    MessageType type;
    /*
     * message sender id
     * server id should be set to 0
     * others' id should be given by server
     */
    long senderID;
    /* message size in bytes
     * it should be automatically calculated when we set message content
     */
protected:
    int size;
public:
    SimpleMessage();
    SimpleMessage(MessageType type, int senderID);

    void setType(MessageType type);
    MessageType getMessageType() const;
    void setSenderID(long id);
    long getSenderID() const;
    int getMessageSize() const;
};


#endif //RING_SIMPLEMESSAGE_H
