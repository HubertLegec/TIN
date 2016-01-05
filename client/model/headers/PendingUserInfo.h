//
// Created by hubert on 05.01.16.
//

#ifndef RING_PENDINGUSERINFO_H
#define RING_PENDINGUSERINFO_H

#include <string>

class PendingUserInfo {
private:
    long categoryID;
    long userID;
    std::string userName;
    std::string categoryName;
public:
    PendingUserInfo();

    PendingUserInfo(long categoryID, long userID, const std::string &userName, const std::string &categoryName);

    void setCategoryID();

    long getCategoryID() const;

    void setUserID();

    long getUserID() const;

    void setUserName(const std::string &userName);

    const std::string &getUserName() const;

    void setCategoryName(const std::string &categoryName);

    const std::string &getCategoryName() const;
};


#endif //RING_PENDINGUSERINFO_H
