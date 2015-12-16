//
// Created by hubert on 05.12.15.
//

#ifndef RING_CATEGORYMANAGEMENTMESSAGE_H
#define RING_CATEGORYMANAGEMENTMESSAGE_H

#include <string>
#include "SimpleMessage.h"
#include "../../cereal/archives/binary.hpp"
#include "../../cereal/types/string.hpp"

/*
 * One class used to create and destroy categories
 * Moreover thanks to it we can sign up, sign out, join and left categories
 *
 */
class CategoryManagementMessage : public SimpleMessage {
private:
    long categoryID;
    std::string categoryName;
    long userID;
    std::string userName;
public:
    CategoryManagementMessage();
    //this constructor should be used to sign up a category when user doesn't have ID
    CategoryManagementMessage(long senderID, MessageType type, long categoryID, const std::string& userName);
    //this constructor should be used to join, left, sign out or destroy the category - user has ID
    CategoryManagementMessage(long senderID, MessageType type, long categoryID, long userID);
    //this constructor should be used to create category
    CategoryManagementMessage(long senderID, MessageType type, const std::string& categoryName, const std::string& userName);
    std::string getCategoryName() const;
    std::string getUserName() const;
    long getCategoryID() const;
    long getUserID() const;
    std::string toString();
    template<class Archive>
    void serialize(Archive & archive){
        archive(this->type, this->size, this->senderID, this->categoryID, this->userID, this->categoryName, this->userName);
    }
};


#endif //RING_CATEGORYMANAGEMENTMESSAGE_H
