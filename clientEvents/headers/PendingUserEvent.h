//
// Created by hubert on 05.01.16.
//

#ifndef RING_PENDINGUSEREVENT_H
#define RING_PENDINGUSEREVENT_H

#include "BasicEvent.h"

class PendingUserEvent : public BasicEvent {
public:
    enum Action {
        ACCEPT = 0,
        REJECT = 1
    };

    PendingUserEvent(long categoryID, long userID, Action action);

    long getCategoryID() const;

    long getUserID() const;

    Action getAction() const;

    virtual std::string toString() const;

    virtual std::string getName() const;

private:
    long categoryID;
    long userID;
    Action action;
};


#endif //RING_PENDINGUSEREVENT_H
