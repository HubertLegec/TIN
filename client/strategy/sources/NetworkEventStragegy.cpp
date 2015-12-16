//
// Created by hubert.legec on 2015-12-15.
//

#include "../headers/NetworkEventStragegy.h"
#include "../../../clientEvents/headers/NetworkEvent.h"
#include "../../../networkMessage/headers/ServerInfoMessage.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"
#include "../../../networkMessage/headers/RingMessage.h"

NetworkEventStragegy::NetworkEventStragegy() : BasicEventStrategy(){ }

NetworkEventStragegy::NetworkEventStragegy(Controller* controller) : BasicEventStrategy(controller) { }

void NetworkEventStragegy::serveMessage(BasicEvent event) {
    NetworkEvent& netEvent = dynamic_cast<NetworkEvent&>(event);
    SimpleMessage msg = netEvent.getMessage();

    switch (msg.getMessageType()){
        case MessageType::SERVER_INFO :
            processServerInfo(msg);
            break;
        case MessageType::CATEGORY_LIST :
            processCategoryList(msg);
            break;
        case MessageType::NEIGHBOURS_SET :
            processNeighbourSet(msg);
            break;
        case MessageType::RING_MESSAGE :
            processRingMessage(msg);
            break;
    }
}

void NetworkEventStragegy::processServerInfo(SimpleMessage message) {
    ServerInfoMessage& msg = dynamic_cast<ServerInfoMessage&>(message);
    //TODO
}

void NetworkEventStragegy::processCategoryList(SimpleMessage message) {
    CategoryListMessage& msg = dynamic_cast<CategoryListMessage&>(message);
    //TODO
}

void NetworkEventStragegy::processNeighbourSet(SimpleMessage message) {
    NeighboursInfoMessage& msg = dynamic_cast<NeighboursInfoMessage&>(message);
    //TODO
}

void NetworkEventStragegy::processRingMessage(SimpleMessage message) {
    RingMessage& msg = dynamic_cast<RingMessage&>(message);
    //TODO
}
