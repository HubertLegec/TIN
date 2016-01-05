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
 * Client use this class to send message when user accept or reject another user request to join to category
 */
class CategoryManagementMessage : public SimpleMessage {
private:
    long categoryID;
    long extraInfo;
    std::string categoryName;
public:
    const static int UNDEFINED_ID = -1;

    CategoryManagementMessage();

    //this constructor should be used to join, left, sign out or destroy the category
    CategoryManagementMessage(long senderID, MessageType type, long categoryID);

    //this constructor should be used to create new category
    CategoryManagementMessage(long senderID, MessageType type, const std::string &categoryName);

    //this constructor should be used to confirm or reject user who want to join to category
    CategoryManagementMessage(long senderID, MessageType type, long categoryID, long extraInfo);

    const std::string &getCategoryName() const;

    long getCategoryID() const;

    void setExtraInfo(long extraInfo);

    long getExtraInfo() const;

    virtual std::string toString() const;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(this->type, this->size, this->senderID, this->categoryID, this->categoryName, this->extraInfo);
    }
};


#endif //RING_CATEGORYMANAGEMENTMESSAGE_H
