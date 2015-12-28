//
// Created by radek on 12/28/15.
//

#ifndef RING_USERMANAGEMENTMESSAGE_H
#define RING_USERMANAGEMENTMESSAGE_H


#include "SimpleMessage.h"

using namespace std;

class UserManagementMessage : public SimpleMessage {
private:
    int port;
    string ip;
    string login;

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

    const string &getLogin() const {
        return login;
    }
};


#endif //RING_USERMANAGEMENTMESSAGE_H
