//
// Created by hubert on 05.01.16.
//

#ifndef RING_PENDINGUSEREVENTSTRATEGY_H
#define RING_PENDINGUSEREVENTSTRATEGY_H

#include "BasicEventStrategy.h"

class PendingUserEventStrategy : public BasicEventStrategy {
private:
    void acceptUser(long categoryID, long userID);

    void rejectUser(long categoryID, long userID);

public:
    PendingUserEventStrategy();

    PendingUserEventStrategy(Controller *controller);

    virtual void serveEvent(BasicEvent *event) override;
};


#endif //RING_PENDINGUSEREVENTSTRATEGY_H
