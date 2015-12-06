//
// Created by hubert on 06.12.15.
//

#include <cstring>
#include "../headers/RingMessage.h"

RingMessage::RingMessage(long senderID, const std::string &text) : SimpleMessage(MessageType::RING_MESSAGE, senderID) {
    this->msgTextSize = text.size();
    this->msgText = new char[msgTextSize + 1];
    strcpy(msgText, text.c_str());
    this->confirmationListSize = 0;
    this->maxConfirmationTextSize = 0;
    this->confirmationList = nullptr;

    this->size = SimpleMessage::getMessageSize() + sizeof(msgTextSize) + msgTextSize + 1
                 + sizeof(confirmationListSize) + sizeof(confirmationList);
}

RingMessage::RingMessage(char *data) : SimpleMessage(data) {
    //TODO
}

RingMessage::~RingMessage() {
    delete[] msgText;
    if(confirmationListSize > 0){
        for(int i = 0; i < confirmationListSize; i++){
            delete[] confirmationList[i];
        }
        delete[] confirmationList;
    }
}

RingMessage& RingMessage::operator=(const RingMessage &other) {
    //TODO
}

void RingMessage::addConfirmation(const std::string &name) {
    if(confirmationList == nullptr){
        confirmationList = new char*[1];
        confirmationList[0] = new char[maxConfirmationTextSize + 1];
        strcpy(confirmationList[0], name.c_str());
        confirmationListSize = 1;
        maxConfirmationTextSize = name.size();
    } else {
        char **temp = new char *[confirmationListSize + 1];

        if(name.size() <= maxConfirmationTextSize) {
            for (int i = 0; i < confirmationListSize; i++) {
                temp[i] = confirmationList[i];
            }
        } else{
            maxConfirmationTextSize = name.size();
            for (int i = 0; i < confirmationListSize; i++) {
                char* tmpName = new char[maxConfirmationTextSize + 1];
                strcpy(tmpName, confirmationList[i]);
                delete[] confirmationList[i];
                temp[i] = tmpName;
            }
        }

        temp[confirmationListSize] = new char[maxConfirmationTextSize + 1];
        delete[] confirmationList;
        confirmationList = temp;
        confirmationListSize++;
    }
}

void RingMessage::addConfirmations(const std::vector<std::string> &nameList) {
    if(confirmationList == nullptr){
        confirmationListSize = nameList.size();
        confirmationList = new char*[confirmationListSize];
        for(std::string name : nameList){
            maxConfirmationTextSize = maxConfirmationTextSize >= name.size() ? maxConfirmationTextSize : name.size();
        }
        for(int i = 0; i < confirmationListSize; i++){
            confirmationList[i] = new char[maxConfirmationTextSize + 1];
            strcpy(confirmationList[i], nameList.at(i).c_str());
        }
    } else {
        char** temp = new char*[confirmationListSize + nameList.size()];
        int newListMax = 0;
        for(std::string name : nameList){
            newListMax = newListMax >= name.size() ? newListMax : name.size();
        }

        if(newListMax <= maxConfirmationTextSize){
            for (int i = 0; i < confirmationListSize; i++) {
                temp[i] = confirmationList[i];
            }
        } else {
            for (int i = 0; i < confirmationListSize; i++) {
                char* tmpName = new char[maxConfirmationTextSize + 1];
                strcpy(tmpName, confirmationList[i]);
                delete[] confirmationList[i];
                temp[i] = tmpName;
            }
        }

        for(int i = 0; i < nameList.size(); i++){
            temp[confirmationListSize + i] = new char[maxConfirmationTextSize + 1];
            strcpy(temp[confirmationListSize + i], nameList.at(i).c_str());
        }
        delete[] confirmationList;
        confirmationList = temp;
        confirmationListSize += nameList.size();
    }
}

std::vector<std::string> RingMessage::getConfirmationsList() const {
    std::vector<std::string> result;
    for(int i = 0; i < confirmationListSize; i++){
        result.push_back(confirmationList[i]);
    }

    return result;
}

std::string RingMessage::getMsgText() const {
    return std::string(msgText);
}


