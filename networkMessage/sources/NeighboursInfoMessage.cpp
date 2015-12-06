//
// Created by hubert on 06.12.15.
//

#include <cstring>
#include "../headers/NeighboursInfoMessage.h"

NeighboursInfoMessage::NeighboursInfoMessage(const std::string& lNeighbourName, const std::string& rNeighbourName)
                                            : SimpleMessage(MessageType::NEIGHBOURS_SET, 0){
    this->lNeighbourNameSize = lNeighbourName.size();
    this->lNeighbourName = new char[lNeighbourNameSize + 1];
    strcpy(this->lNeighbourName, lNeighbourName.c_str());
    this->rNeighbourNameSize = rNeighbourName.size();
    this->rNeighbourName = new char[rNeighbourNameSize + 1];
    strcpy(this->rNeighbourName, rNeighbourName.c_str());

    this->size = SimpleMessage::getMessageSize() + 2*sizeof(int) + lNeighbourNameSize + rNeighbourNameSize + 2;
}

NeighboursInfoMessage::NeighboursInfoMessage(char *data) : SimpleMessage(data) {
    //TODO
}

NeighboursInfoMessage::~NeighboursInfoMessage() {
    delete[] lNeighbourName;
    delete[] rNeighbourName;
}

std::string NeighboursInfoMessage::getLeftNeighbourName() const {
    return std::string(lNeighbourName);
}

std::string NeighboursInfoMessage::getRightNeighbourName() const {
    return std::string(rNeighbourName);
}