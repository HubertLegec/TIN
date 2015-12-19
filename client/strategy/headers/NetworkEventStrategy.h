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
    void serveMessage(BasicEvent event);

private:
    void processServerInfo(SimpleMessage message);
    void processCategoryList(SimpleMessage message);
    void processNeighbourSet(SimpleMessage message);
    void processRingMessage(SimpleMessage message);

    void serveServerInfoCategoryCreated(const ServerInfoMessage& msg);
    void serveServerInfoCategoryRemoved(const ServerInfoMessage& msg);
    void serveServerInfoUserAdded(const ServerInfoMessage& msg);
    void serveServerInfoFail(const ServerInfoMessage& msg);
};


#endif //RING_NETWORKEVENTSTRAGEGY_H
