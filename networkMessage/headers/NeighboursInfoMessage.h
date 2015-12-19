//
// Created by hubert on 06.12.15.
//

#ifndef RING_NEIGHBOURSINFOMESSAGE_H
#define RING_NEIGHBOURSINFOMESSAGE_H

#include <string>
#include "SimpleMessage.h"
#include "../../cereal/archives/binary.hpp"
#include "../../cereal/types/string.hpp"

class NeighboursInfoMessage : public SimpleMessage {
private:
    long categoryId;
    std::string lNeighbourName;
    std::string lNeighbourIP;
    int lNeighbourPort;
    std::string rNeighbourName;
    std::string rNeighbourIP;
    int rNeighbourPort;
public:
    NeighboursInfoMessage();
    NeighboursInfoMessage(long categoryId, const std::string& lNeighName, const std::string& lNeighIP, int lNeighPort,
                          const std::string& rNeighName, const std::string& rNeighIP, int rNeighPort);
    std::string getLeftNeighbourName() const;
    std::string getLeftNeighbourIP() const;
    int getLeftNeighbourPort() const;
    std::string getRightNeighbourName() const;
    std::string getRightNeighbourIP() const;
    int getRightNeighbourPort() const;
    long getCategoryId() const;
    std::string toString();
    template<class Archive>
    void serialize(Archive & archive){
        archive(this->type, this->size, this->senderID, this->categoryId, this->lNeighbourName, this->lNeighbourIP, this->lNeighbourPort,
                this->rNeighbourName, this->rNeighbourIP, this->rNeighbourPort);
    }
};


#endif //RING_NEIGHBOURSINFOMESSAGE_H
