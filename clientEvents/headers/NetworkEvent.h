//
// Created by hubert on 15.12.15.
//

#ifndef RING_NETWORKEVENT_H
#define RING_NETWORKEVENT_H

#include "../../networkMessage/headers/SimpleMessage.h"


class NetworkEvent : public BasicEvent {
private:
    SimpleMessage message;

public:
    NetworkEvent(SimpleMessage message);

    SimpleMessage getMessage() const;

};


#endif //RING_NETWORKEVENT_H
