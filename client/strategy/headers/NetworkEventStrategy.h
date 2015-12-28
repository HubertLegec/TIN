//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_NETWORKEVENTSTRATEGY_H
#define RING_NETWORKEVENTSTRATEGY_H

#include "BasicEventStrategy.h"
#include "../../../networkMessage/headers/SimpleMessage.h"

class NetworkEventStrategy : public BasicEventStrategy {
public:
    NetworkEventStrategy();
    NetworkEventStrategy(Controller* controller);

    virtual void serveEvent(BasicEvent *event) const;

private:
    void processServerInfo(SimpleMessage &message) const;

    void processCategoryList(SimpleMessage &message) const;

    void processNeighbourSet(SimpleMessage &message) const;

    void processRingMessage(SimpleMessage &message) const;
};


#endif //RING_NETWORKEVENTSTRATEGY_H
