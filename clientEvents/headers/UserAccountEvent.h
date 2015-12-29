//
// Created by hubert on 29.12.15.
//

#ifndef RING_USERACCOUNTEVENT_H
#define RING_USERACCOUNTEVENT_H

#include <string>
#include "BasicEvent.h"


class UserAccountEvent : public BasicEvent {
private:
    std::string userName;

public:
    UserAccountEvent(const std::string &userName);

    const std::string &getUserName() const;

    virtual std::string toString() const;

    virtual std::string getName() const;
};


#endif //RING_USERACCOUNTEVENT_H
