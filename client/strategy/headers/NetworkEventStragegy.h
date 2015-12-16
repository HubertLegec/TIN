//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_NETWORKEVENTSTRAGEGY_H
#define RING_NETWORKEVENTSTRAGEGY_H

#include "BasicEventStrategy.h"
#include "../../../networkMessage/headers/SimpleMessage.h"

class NetworkEventStragegy : public BasicEventStrategy {
public:
    NetworkEventStragegy();
    NetworkEventStragegy(Controller* controller);
    void serveMessage(BasicEvent event);

private:
    void processServerInfo(SimpleMessage message);
    void processCategoryList(SimpleMessage message);
    void processNeighbourSet(SimpleMessage message);
    void processRingMessage(SimpleMessage message);
};


#endif //RING_NETWORKEVENTSTRAGEGY_H
