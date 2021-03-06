//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CONFIRMMESSAGEEVENTSTRATEGY_H
#define RING_CONFIRMMESSAGEEVENTSTRATEGY_H

#include "BasicEventStrategy.h"


class ConfirmMessageEventStrategy : public BasicEventStrategy {

public:
    ConfirmMessageEventStrategy();
    ConfirmMessageEventStrategy(Controller* controller);

    virtual void serveEvent(BasicEvent *event) override;
};


#endif //RING_CONFIRMMESSAGEEVENTSTRATEGY_H
