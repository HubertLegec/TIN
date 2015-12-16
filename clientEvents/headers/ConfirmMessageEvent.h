//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CONFIRMMESSAGEEVENT_H
#define RING_CONFIRMMESSAGEEVENT_H

#include "BasicEvent.h"

class ConfirmMessageEvent : public BasicEvent {


public:
    ConfirmMessageEvent();
    std::string toString();
};


#endif //RING_CONFIRMMESSAGEEVENT_H
