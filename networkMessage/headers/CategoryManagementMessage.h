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
public:
    const static int UNDEFINED_ID = -1;

    CategoryManagementMessage();

    //this constructor should be used to join, left, sign out or destroy the category
    CategoryManagementMessage(long senderID, MessageType type, long categoryID);

    //this constructor should be used to create new category
    CategoryManagementMessage(long senderID, MessageType type, const std::string &categoryName);
    std::string getCategoryName() const;
    long getCategoryID() const;
    std::string toString();
    template<class Archive>
    void serialize(Archive & archive){
        archive(this->type, this->size, this->senderID, this->categoryID, this->categoryName);
    }
};


#endif //RING_CATEGORYMANAGEMENTMESSAGE_H
