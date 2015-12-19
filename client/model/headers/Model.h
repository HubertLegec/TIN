//
// Created by hubert on 03.12.15.
//

#ifndef RING_MODEL_H
#define RING_MODEL_H

#include <string>
#include <map>
#include <set>
#include <vector>
#include "ConnectionInfo.h"
#include "CategoryInfo.h"
#include "../../../networkMessage/headers/RingMessage.h"

class Model {
private:
    static const std::string SERVER_DEFAULT_IP = "localhost";
    static const int SERVER_DEFAULT_PORT = "8888";

    long userID;
    std::string userName;

    ConnectionInfo serverInfo;

    std::map<std::string, long> categoryNameIdMapping;

    std::vector<RingMessage> inbox;

    /**
     * key - category id
     * value - category data
     */
    std::map<long, CategoryInfo> categories;

public:
    Model();
    void setServerInfo(std::string ip, int port);
    ConnectionInfo getServerInfo() const;
    std::string getCategoryName(long id) const;
    long getCategoryId(std::string name) const;
    void setUserName(std::string userName);
    std::string getUserName() const;
    void setUserId(long userId);
    long getUserId() const;
    void addMessageToCategory(long categoryId, std::string message);
    std::vector<std::string> getCategoryMessages(long categoryId) const;
    void updateLeftNeighbour(long categoryId, const ConnectionInfo& info);
    void updateRightNeighbour(long categoryId, const ConnectionInfo& info);
    ConnectionInfo getLeftNeighbour(long categoryId) const;
    ConnectionInfo getRightNeighbour(long categoryId) const;
    void addMyCategory(long id, std::string name);
    void addJoinedCategory(long id, std::string name);
    void removeCategoryAndData(long id);
    std::vector<std::pair<long, std::string>> getMyCategories() const;
    std::vector<std::pair<long, std::string>> getJoinedCategories() const;
    bool isCategoryActive(long categoryId) const;
    void setCategoryActive(long categoryId, bool active);
    void addMessageToInbox(RingMessage message);
    std::vector<RingMessage> getInboxMessages() const;
    void markMessageAsRead(long messageIndex);
};


#endif //RING_MODEL_H
