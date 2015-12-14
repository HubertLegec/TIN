//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/CategoryListMessage.h"

CategoryListMessage::CategoryListMessage() : SimpleMessage() { }

CategoryListMessage::CategoryListMessage(long senderID, const std::map<long, std::string>& categories)
                                        : SimpleMessage(MessageType::CATEGORY_LIST, senderID), categories(categories) {

    this->size = SimpleMessage::getMessageSize() + sizeof(long) + categories.size()*sizeof(long);
    for(std::pair<long, std::string> p : categories){
        size += sizeof(long) + p.second.size();
    }
}

std::map<long, std::string> CategoryListMessage::getCategories() const {
    return categories;
}

void CategoryListMessage::addCategory(long categoryID, std::string categoryName) {
    categories.insert(std::pair<long, std::string>(categoryID, categoryName));
    size += (sizeof(long) + categoryName.size());
}


