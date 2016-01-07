//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/CategoryListMessage.h"

using namespace std;

CategoryListMessage::CategoryListMessage() : SimpleMessage() {
    this->size = SimpleMessage::getMessageSize() + sizeof(long);
}

CategoryListMessage::CategoryListMessage(long senderID, const map<long, string> &categories)
        : SimpleMessage(MessageType::CATEGORY_LIST, senderID), categories(categories) {

    this->size = SimpleMessage::getMessageSize() + sizeof(long) + categories.size() * sizeof(long);
    for (pair<long, string> p : categories) {
        size += (sizeof(long) + p.second.size());
    }
}

map<long, string> CategoryListMessage::getCategories() const {
    return categories;
}

void CategoryListMessage::addCategory(long categoryID, const string &categoryName) {
    categories.insert(pair<long, string>(categoryID, categoryName));
    size += (sizeof(long) + categoryName.size());
}

string CategoryListMessage::toString() const {
    stringstream ss;
    ss << "CategoryListMessage[type:" << type << "; ";
    ss << "size:" << size << "; ";
    ss << "senderID:" << senderID << "; ";
    ss << "categories:{";
    for (auto p : categories) {
        ss << endl << "id:" << p.first << ", name:" << p.second;
    }
    ss << "}]";
    return ss.str();
}