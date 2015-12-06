//
// Created by hubert on 06.12.15.
//

#ifndef RING_RINGMESSAGE_H
#define RING_RINGMESSAGE_H

#include <string>
#include <vector>
#include "SimpleMessage.h"

class RingMessage : public SimpleMessage {
private:
    int msgTextSize;
    char* msgText;
    int confirmationListSize;
    int maxConfirmationTextSize;
    char** confirmationList;
public:
    RingMessage(long senderID, const std::string& text);
    RingMessage(char* data);
    ~RingMessage();
    RingMessage& operator=(const RingMessage& other);
    std::vector<std::string> getConfirmationsList() const;
    void addConfirmation(const std::string& name);
    void addConfirmations(const std::vector<std::string>& nameList);
    std::string getMsgText() const;
};


#endif //RING_RINGMESSAGE_H
