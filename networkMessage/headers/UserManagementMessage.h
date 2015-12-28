//
// Created by radek on 12/28/15.
//

#ifndef RING_USERMANAGEMENTMESSAGE_H
#define RING_USERMANAGEMENTMESSAGE_H

#include "../../cereal/archives/binary.hpp"
#include "../../cereal/types/string.hpp"
#include "SimpleMessage.h"

using namespace std;

class UserManagementMessage : public SimpleMessage {
private:
    int port;
    string ip;
    string userName;

    static const long UNDEFINED_ID = -1;

public:
    // To sign up user
    UserManagementMessage(MessageType type, string ip, int port, string login);

    // To delete user
    UserManagementMessage(long senderID, MessageType type);

    int getPort() const {
        return port;
    }

    const string &getIp() const {
        return ip;
    }

    const string &getUserName() const {
        return userName;
    }

    template<class Archive>
    void serialize(Archive & archive){
        archive(this->type, this->size, this->senderID, this->port, this->ip, this->userName);
    }
};


#endif //RING_USERMANAGEMENTMESSAGE_H
