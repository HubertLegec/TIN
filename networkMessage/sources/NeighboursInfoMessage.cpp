//
// Created by hubert on 06.12.15.
//

#include "../headers/NeighboursInfoMessage.h"

using namespace std;

NeighboursInfoMessage::NeighboursInfoMessage() : SimpleMessage() {
    this->size = SimpleMessage::getMessageSize() + 2 * sizeof(int) + 5 * sizeof(long);
}

NeighboursInfoMessage::NeighboursInfoMessage(long categoryId, const string &lNeighName, const string &lNeighIP,
                                             int lNeighPort,
                                             const string &rNeighName, const string &rNeighIP, int rNeighPort)
        : SimpleMessage(MessageType::NEIGHBOURS_SET, 0) {
    this->categoryId = categoryId;
    this->lNeighbourName = lNeighName;
    this->lNeighbourIP = lNeighIP;
    this->lNeighbourPort = lNeighPort;
    this->rNeighbourName = rNeighName;
    this->rNeighbourIP = rNeighIP;
    this->rNeighbourPort = rNeighPort;
    this->size = SimpleMessage::getMessageSize() + 2 * sizeof(int) + 5 * sizeof(long)
                 + lNeighName.size() + rNeighName.size() + lNeighIP.size() + rNeighIP.size();
}

const string &NeighboursInfoMessage::getLeftNeighbourName() const {
    return lNeighbourName;
}

const string &NeighboursInfoMessage::getLeftNeighbourIP() const {
    return lNeighbourIP;
}

int NeighboursInfoMessage::getLeftNeighbourPort() const {
    return lNeighbourPort;
}

const string &NeighboursInfoMessage::getRightNeighbourName() const {
    return rNeighbourName;
}

const string &NeighboursInfoMessage::getRightNeighbourIP() const {
    return rNeighbourIP;
}

int NeighboursInfoMessage::getRightNeighbourPort() const {
    return rNeighbourPort;
}

long NeighboursInfoMessage::getCategoryId() const {
    return categoryId;
}

string NeighboursInfoMessage::toString() const {
    stringstream ss;
    ss << "NeighbourInfoMessage[type:" << type << "; ";
    ss << "size:" << size << "; ";
    ss << "senderID:" << senderID << "; ";
    ss << "categoryID:" << categoryId << ";\n";
    ss << "lNeighName:" << lNeighbourName << "; ";
    ss << "lNeighIP:" << lNeighbourIP << "; ";
    ss << "lNeighPort:" << lNeighbourPort << ";\n";
    ss << "rNeighName:" << rNeighbourName << "; ";
    ss << "rNeighIP:" << rNeighbourIP << "; ";
    ss << "rNeighPort:" << rNeighbourPort << "]";
    return ss.str();
}