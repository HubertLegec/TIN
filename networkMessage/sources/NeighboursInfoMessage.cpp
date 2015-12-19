//
// Created by hubert on 06.12.15.
//

#include "../headers/NeighboursInfoMessage.h"

NeighboursInfoMessage::NeighboursInfoMessage() : SimpleMessage(){ }

NeighboursInfoMessage::NeighboursInfoMessage(long categoryId, const std::string& lNeighName, const std::string& lNeighIP, int lNeighPort,
                                             const std::string& rNeighName, const std::string& rNeighIP, int rNeighPort)
                                            : SimpleMessage(MessageType::NEIGHBOURS_SET, 0){
    this->categoryId = categoryId;
    this->lNeighbourName = lNeighName;
    this->lNeighbourIP = lNeighIP;
    this->lNeighbourPort = lNeighPort;
    this->rNeighbourName = rNeighName;
    this->rNeighbourIP = rNeighIP;
    this->rNeighbourPort = rNeighPort;

    this->size = SimpleMessage::getMessageSize() + 2*sizeof(int) + 5*sizeof(long)
                 + lNeighName.size() + rNeighName.size() + lNeighIP.size() + rNeighIP.size();
}

std::string NeighboursInfoMessage::getLeftNeighbourName() const {
    return lNeighbourName;
}

std::string NeighboursInfoMessage::getLeftNeighbourIP() const {
    return lNeighbourIP;
}

int NeighboursInfoMessage::getLeftNeighbourPort() const {
    return lNeighbourPort;
}

std::string NeighboursInfoMessage::getRightNeighbourName() const {
    return rNeighbourName;
}

std::string NeighboursInfoMessage::getRightNeighbourIP() const {
    return rNeighbourIP;
}

int NeighboursInfoMessage::getRightNeighbourPort() const {
    return rNeighbourPort;
}

long NeighboursInfoMessage::getCategoryId() const {
    return categoryId;
}

std::string NeighboursInfoMessage::toString() {
    //TODO
    return "NeighbourInfoMessage";
}