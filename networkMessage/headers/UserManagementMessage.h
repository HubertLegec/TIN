//
// Created by radek on 12/28/15.
//

#ifndef RING_USERMANAGEMENTMESSAGE_H
#define RING_USERMANAGEMENTMESSAGE_H

#include "../../cereal/archives/binary.hpp"
#include "../../cereal/types/string.hpp"
#include "SimpleMessage.h"

class UserManagementMessage : public SimpleMessage {
private:
    int port;
    std::string ip;
    std::string userName;

    static const long UNDEFINED_ID = -1;

public:
    // To sign up user
    UserManagementMessage(MessageType type, std::string ip, int port, std::string login);

    // To delete user
    UserManagementMessage(long senderID, MessageType type);

    int getPort() const;

    const std::string &getIp() const;

    const std::string &getUserName() const;

    virtual std::string toString() const;

    template<class Archive>
    void serialize(Archive & archive){
        archive(this->type, this->size, this->senderID, this->port, this->ip, this->userName);
    }
};


#endif //RING_USERMANAGEMENTMESSAGE_H
