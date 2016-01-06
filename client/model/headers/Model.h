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
#include "PendingUserInfo.h"
#include "../../../networkMessage/headers/RingMessage.h"

class Model {
private:

    long userID;
    std::string userName;
    int myPort;
    std::string myIP;

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
    static constexpr const char *SERVER_DEFAULT_IP = "127.0.1.1";
    static const int SERVER_DEFAULT_PORT = 8888;
    static constexpr const char *CLIENT_DEFAULT_IP = "127.0.1.1";
    static const int CLIENT_DEFAULT_PORT = 7777;
    static const long UNDEFINED_ID = -1;

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

    void updateLeftNeighbour(long categoryId, const ConnectionInfo &info);

    const ConnectionInfo &getLeftNeighbour(long categoryId);

    void addMyCategory(long id, const std::string &name);

    void addJoinedCategory(long id, const std::string &name);

    void removeCategoryAndData(long id);

    std::map<long, std::string> getCategories() const;

    std::map<long, std::string> getMyCategories() const;

    std::map<long, std::string> getJoinedCategories() const;

    bool isCategoryActive(long categoryId) const;

    void setCategoryActive(long categoryId, bool active);

    std::map<long, std::string> getActiveCategories() const;

    std::map<long, std::string> getInactiveCategories() const;

    void addMessageToInbox(const RingMessage &message);

    std::vector<RingMessage> getInboxMessages() const;

    void markMessageAsRead(long messageIndex);

    void addNotification(const std::string &notification);

    std::vector<std::string> getNotifications();

    void clearNotificationList();

    void setMyIP(const std::string &ip);

    void setMyPort(int port);

    const std::string &getMyIP() const;

    int getMyPort() const;

    bool isRegistered() const;

    bool isMyCategory(long categoryID) const;

    void confirmCategory(long categoryID);

    void addPendingUser(long categoryID, long userID, const std::string &userName);

    void removePendingUser(long categoryID, long userID);

    std::vector<PendingUserInfo> getPendingUsers() const;

    void removeUserAccount();

    bool isSomeoneInCategory(long categoryID) const;

    std::map<long, std::string> getMyNonEmptyCategories() const;

    std::vector<std::string> getMyEmptyAndNonActiveCategories() const;
};


#endif //RING_MODEL_H
