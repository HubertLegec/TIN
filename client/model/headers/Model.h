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

    long userID;
    std::string userName;

    ConnectionInfo serverInfo;

    std::map<std::string, long> categoryNameIdMapping;

    std::vector<RingMessage> inbox;

    std::vector<std::string> notifications;

    /**
     * key - category id
     * value - category data
     */
    std::map<long, CategoryInfo> categories;

public:
    static constexpr const char* SERVER_DEFAULT_IP = "127.0.0.1";
    static const int SERVER_DEFAULT_PORT = 8888;

    Model();

    void setServerInfo(const std::string &ip, int port);

    const ConnectionInfo &getServerInfo() const;

    const std::string &getCategoryName(long id) const;

    long getCategoryId(const std::string &name) const;

    void setUserName(const std::string &userName);

    const std::string &getUserName() const;
    void setUserId(long userId);
    long getUserId() const;

    void addMessageToCategory(long categoryId, const std::string &message);
    std::vector<std::string> getCategoryMessages(long categoryId);
    void updateLeftNeighbour(long categoryId, const ConnectionInfo& info);
    void updateRightNeighbour(long categoryId, const ConnectionInfo& info);

    const ConnectionInfo &getLeftNeighbour(long categoryId);

    const ConnectionInfo &getRightNeighbour(long categoryId);

    void addMyCategory(long id, const std::string &name);

    void addJoinedCategory(long id, const std::string &name);
    void removeCategoryAndData(long id);
    std::vector<std::pair<long, std::string>> getMyCategories() const;
    std::vector<std::pair<long, std::string>> getJoinedCategories() const;
    bool isCategoryActive(long categoryId) const;
    void setCategoryActive(long categoryId, bool active);

    void addMessageToInbox(const RingMessage &message);
    std::vector<RingMessage> getInboxMessages() const;
    void markMessageAsRead(long messageIndex);
    void addNotification(const std::string& notification);
    std::vector<std::string> getNotifications();
    void clearNotificationList();
};


#endif //RING_MODEL_H
