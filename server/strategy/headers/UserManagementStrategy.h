#ifndef RING_USERMANAGEMENTSTRATEGY_H
#define RING_USERMANAGEMENTSTRATEGY_H

#include "BasicEventStrategy.h"

class UserManagementStrategy : public BasicEventStrategy {
private:

public:
    UserManagementStrategy() : BasicEventStrategy() { }

    UserManagementStrategy(Controller *controller) : BasicEventStrategy(controller) { }

    virtual void serveEvent(SimpleMessage *message) const;

};


#endif //RING_USERMANAGEMENTSTRATEGY_H
