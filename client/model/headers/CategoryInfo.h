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
    std::vector<std::string> messages;
    std::map<long, std::string> pendingUsers;
    bool active;
    bool owner;
    bool confirmed;
public:
    CategoryInfo();

    CategoryInfo(const std::string &name, bool owner = false);

    void updateLeftNeighbour(const ConnectionInfo &info);

    const ConnectionInfo &getLeftNeighbour() const;

    void setOwner(bool owner);

    bool isOwner() const;

    void setActive(bool active);

    bool isActive() const;

    void setConfirmed(bool confirmed);

    bool isConfirmed() const;

    void addMessage(const std::string &message);

    void addPendingUser(long userID, std::string userName);

    void removePendingUser(long userID);

    std::map<long, std::string> getPendingUsers() const;

    std::vector<std::string> getMessages() const;

    const std::string &getName() const;
};


#endif //RING_CATEGORYINFO_H