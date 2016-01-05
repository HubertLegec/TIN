//
// Created by hubert on 29.12.15.
//

#ifndef RING_USERACCOUNTEVENT_H
#define RING_USERACCOUNTEVENT_H

#include <string>
#include "BasicEvent.h"

/**
 * This class is used to transfer events generated when user account is created or removed
 */
class UserAccountEvent : public BasicEvent {
public:
    enum Type {
        CREATE = 0,
        DELETE = 1
    };
private:
    Type type;
    std::string userName;

public:

    UserAccountEvent(Type type, const std::string &userName);

    const std::string &getUserName() const;

    Type getType() const;

    virtual std::string toString() const;

    virtual std::string getName() const;
};


#endif //RING_USERACCOUNTEVENT_H
