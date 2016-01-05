//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CONFIRMMESSAGEEVENT_H
#define RING_CONFIRMMESSAGEEVENT_H

#include "BasicEvent.h"

/**
 * This class is used to transfer all the events connected with confirming messages(marking them as read)
 */
class ConfirmMessageEvent : public BasicEvent {
private:
    //current index of message in inbox
    long messageIndex;

public:
    ConfirmMessageEvent();

    ConfirmMessageEvent(long messageIndex);

    long getMessageIndex() const;

    void setMessageIndex(long messageIndex);

    virtual std::string toString() const;

    virtual std::string getName() const;
};

#endif //RING_CONFIRMMESSAGEEVENT_H