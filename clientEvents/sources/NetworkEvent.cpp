//
// Created by hubert on 15.12.15.
//

#include <sstream>
#include "../headers/NetworkEvent.h"

using namespace std;

NetworkEvent::NetworkEvent(shared_ptr<SimpleMessage> message) : BasicEvent(), message(message) {

}

std::shared_ptr<SimpleMessage> NetworkEvent::getMessage() const {
    return message;
}

string NetworkEvent::toString() const {
    stringstream ss;
    ss << "NetworkEvent[";
    ss << message.get()->toString();
    ss << "]";
    return ss.str();
}

string NetworkEvent::getName() const {
    return "NETWORK_EVENT";
}
