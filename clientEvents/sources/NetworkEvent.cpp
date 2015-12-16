//
// Created by hubert on 15.12.15.
//

#include "../headers/NetworkEvent.h"

NetworkEvent::NetworkEvent(SimpleMessage message) : message(message) {

}

SimpleMessage NetworkEvent::getMessage() const {
    return message;
}

std::string NetworkEvent::toString() {
    //TODO
    return "NetworkEvent";
}
