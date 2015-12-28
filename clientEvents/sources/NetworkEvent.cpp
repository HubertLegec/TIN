//
// Created by hubert on 15.12.15.
//

#include "../headers/NetworkEvent.h"

NetworkEvent::NetworkEvent(std::shared_ptr<SimpleMessage> message) : message(message) {

}

std::shared_ptr<SimpleMessage> NetworkEvent::getMessage() const {
    return message;
}

std::string NetworkEvent::toString() {
    //TODO
    return "NetworkEvent";
}
