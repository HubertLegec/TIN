//
// Created by hubert on 15.12.15.
//

#ifndef RING_NETWORKEVENT_H
#define RING_NETWORKEVENT_H

#include "../../networkMessage/headers/SimpleMessage.h"
#include "BasicEvent.h"


class NetworkEvent : public BasicEvent {
private:
    std::shared_ptr<SimpleMessage> message;

public:
    NetworkEvent(std::shared_ptr<SimpleMessage> message);

    std::shared_ptr<SimpleMessage> getMessage() const;

    virtual std::string toString();

    virtual std::string getName();
};


#endif //RING_NETWORKEVENT_H
