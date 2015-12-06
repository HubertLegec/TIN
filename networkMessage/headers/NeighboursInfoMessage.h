//
// Created by hubert on 06.12.15.
//

#ifndef RING_NEIGHBOURSINFOMESSAGE_H
#define RING_NEIGHBOURSINFOMESSAGE_H

#include <string>
#include "SimpleMessage.h"

class NeighboursInfoMessage : public SimpleMessage {
private:
    int lNeighbourNameSize;
    char* lNeighbourName;
    int rNeighbourNameSize;
    char* rNeighbourName;
public:
    NeighboursInfoMessage(const std::string& lNeighbourName, const std::string& rNeighbourName);
    NeighboursInfoMessage(char* data);
    ~NeighboursInfoMessage();
    std::string getLeftNeighbourName() const;
    std::string getRightNeighbourName() const;
};


#endif //RING_NEIGHBOURSINFOMESSAGE_H
