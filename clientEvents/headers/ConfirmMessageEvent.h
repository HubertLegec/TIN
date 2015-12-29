//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CONFIRMMESSAGEEVENT_H
#define RING_CONFIRMMESSAGEEVENT_H

#include "BasicEvent.h"


class ConfirmMessageEvent : public BasicEvent {
private:
    //index of message in inbox
    long messageIndex;

public:
    ConfirmMessageEvent();
    ConfirmMessageEvent(long messageIndex);
    long getMessageIndex() const;
    void setMessageIndex(long messageIndex);

    virtual std::string toString();

    virtual std::string getName();
};


#endif //RING_CONFIRMMESSAGEEVENT_H
