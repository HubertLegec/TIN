//
// Created by hubert on 05.12.15.
//

#ifndef RING_CATEGORYMANAGEMENTMESSAGE_H
#define RING_CATEGORYMANAGEMENTMESSAGE_H

#include <string>
#include "SimpleMessage.h"

/*
 * One class used to create and destroy categories
 * Moreover thanks to it we can sign up, sign out, join and left categories
 *
 */
class CategoryManagementMessage : public SimpleMessage {
private:
    int categoryNameSize;
    char* categoryName;
    int userNameSize;
    char* userName;
public:
    CategoryManagementMessage(long senderID, MessageType type, const std::string& categoryName, const std::string& userName);
    CategoryManagementMessage(char* data);
    ~CategoryManagementMessage();
    std::string getCategoryName() const;
    std::string getUserName() const;

};


#endif //RING_CATEGORYMANAGEMENTMESSAGE_H
