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
    void serveEvent(BasicEvent event);

private:
    void processServerInfo(SimpleMessage message);
    void processCategoryList(SimpleMessage message);
    void processNeighbourSet(SimpleMessage message);
    void processRingMessage(SimpleMessage message);
};


#endif //RING_NETWORKEVENTSTRATEGY_H
