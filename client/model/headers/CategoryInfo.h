//
// Created by hubert on 19.12.15.
//

#ifndef RING_CATEGORYINFO_H
#define RING_CATEGORYINFO_H

#include "ConnectionInfo.h"
#include <vector>
#include <string>

class CategoryInfo {
private:
    std::string name;
    ConnectionInfo leftNeighbour;
    ConnectionInfo rightNeighbour;
    std::vector<std::string> messages;
    bool active;
    bool owner;
public:
    CategoryInfo();

    CategoryInfo(const std::string &name, bool owner = false);
    void updateRightNeighbour(const ConnectionInfo& info);

    const ConnectionInfo &getRightNeighbour() const;
    void updateLeftNeighbour(const ConnectionInfo& info);

    const ConnectionInfo &getLeftNeighbour() const;
    void setOwner(bool owner);
    bool isOwner() const;
    void setActive(bool active);
    bool isActive() const;
    void addMessage(const std::string& message);
    std::vector<std::string> getMessages() const;

    const std::string &getName() const;
};


#endif //RING_CATEGORYINFO_H
