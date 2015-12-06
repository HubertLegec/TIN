//
// Created by hubert on 05.12.15.
//

#ifndef RING_SERVERINFOMESSAGE_H
#define RING_SERVERINFOMESSAGE_H

#include <string>
#include "SimpleMessage.h"

class ServerInfoMessage : public SimpleMessage {
private:
    ServerInfoMessageType infoType;
    /* additional field - can be used for example for passing added user id */
    long extraInfo;
    int infoSize;
    char* info;
public:
    ServerInfoMessage(long senderID, ServerInfoMessageType infoType, const std::string& info);
    ServerInfoMessage(char* data);
    ~ServerInfoMessage();
    ServerInfoMessageType getInfoType() const;
    void setExtraInfo(long extraInfo);
    long getExtraInfo() const;
    std::string getInfo() const;
    void setInfo(const std::string info);
};


#endif //RING_SERVERINFOMESSAGE_H
