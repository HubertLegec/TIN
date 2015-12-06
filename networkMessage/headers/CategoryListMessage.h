//
// Created by hubert on 05.12.15.
//

#ifndef RING_CATEGORYLISTMESSAGE_H
#define RING_CATEGORYLISTMESSAGE_H

#include <vector>
#include <string>
#include "SimpleMessage.h"

class CategoryListMessage : public SimpleMessage {
private:
    int listSize;
    int maxNameSize;
    /* pointer to an array of category names
     */
    char **catList;

public:
    CategoryListMessage(long senderID, const std::vector<std::string>& list);
    CategoryListMessage(char* data);
    ~CategoryListMessage();
    std::vector<std::string> getCategoryList() const;
};


#endif //RING_CATEGORYLISTMESSAGE_H
