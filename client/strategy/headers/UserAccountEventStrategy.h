//
// Created by hubert on 29.12.15.
//

#ifndef RING_USERACCOUNTEVENTSTRATEGY_H
#define RING_USERACCOUNTEVENTSTRATEGY_H

#include "BasicEventStrategy.h"
#include "../../../clientEvents/headers/BasicEvent.h"

class UserAccountEventStrategy : public BasicEventStrategy {
private:

public:
    UserAccountEventStrategy();

    UserAccountEventStrategy(Controller *controller);

    virtual void serveEvent(BasicEvent *event) override;
};


#endif //RING_USERACCOUNTEVENTSTRATEGY_H
