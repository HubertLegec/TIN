//
// Created by hubert on 02.01.16.
//

#ifndef RING_NEWMESSAGEEVENT_H
#define RING_NEWMESSAGEEVENT_H

#include "BasicEvent.h"


class NewMessageEvent : public BasicEvent {
private:
    long categoryID;
    std::string message;
public:
    NewMessageEvent(long categoryID, const std::string &message);

    long getCategoryID() const;

    const std::string &getMessage() const;

    virtual std::string toString() const;

    virtual std::string getName() const;
};


#endif //RING_NEWMESSAGEEVENT_H
