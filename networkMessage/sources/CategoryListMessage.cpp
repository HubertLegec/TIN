//
// Created by hubert on 05.12.15.
//

#include <cstring>
#include "../headers/CategoryListMessage.h"

CategoryListMessage::CategoryListMessage(long senderID, const std::vector<std::string>& list)
                                        : SimpleMessage(MessageType::CATEGORY_LIST, senderID), listSize(list.size()){

    this->size = SimpleMessage::getMessageSize() + sizeof(listSize) + sizeof(maxNameSize);

    if(list.size() > 0){
        maxNameSize = 0;
        for(std::string name : list){
            maxNameSize = name.size() > maxNameSize ? name.size() : maxNameSize;
        }
        catList = new char*[listSize];
        for(int i = 0; i < listSize; i++){
            catList[i] = new char[maxNameSize+1];
            strcpy(catList[i], list.at(i).c_str());
            this->size += (maxNameSize+1);
        }
    } else {
        this->maxNameSize = 0;
        this->catList = nullptr;
    }
}

CategoryListMessage::CategoryListMessage(char *data) : SimpleMessage(data) {
    //TODO
}

std::vector<std::string> CategoryListMessage::getCategoryList() const {
    std::vector<std::string> result;
    for(int i = 0; i < listSize; i++){
        result.push_back(catList[i]);
    }

    return result;
}

CategoryListMessage::~CategoryListMessage() {
    for(int i = 0; i < listSize; i++){
        delete[] catList[i];
    }
    delete[] catList;
}