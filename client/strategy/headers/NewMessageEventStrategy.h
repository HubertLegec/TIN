//
// Created by hubert on 02.01.16.
//

#ifndef RING_NEWMESSAGEEVENTSTRATEGY_H
#define RING_NEWMESSAGEEVENTSTRATEGY_H

#include "BasicEventStrategy.h"

class NewMessageEventStrategy : public BasicEventStrategy {

public:
    NewMessageEventStrategy();

    NewMessageEventStrategy(Controller *controller);

    virtual void serveEvent(BasicEvent *event) override;
};


#endif //RING_NEWMESSAGEEVENTSTRATEGY_H
