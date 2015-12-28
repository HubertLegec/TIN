#include "../headers/UserManagementMessage.h"

UserManagementMessage::UserManagementMessage(MessageType type, string ip, int port, string login) : SimpleMessage(type,
                                                                                                                   UNDEFINED_ID) {
    this->ip = ip;
    this->port = port;
    this->userName = login;

    this->size = SimpleMessage::getMessageSize() + sizeof(int) + 2*sizeof(long) + userName.size() + ip.size();

}

UserManagementMessage::UserManagementMessage(long senderID, MessageType type) : SimpleMessage(type, senderID) {
    this->size = SimpleMessage::getMessageSize() + sizeof(int) + 2*sizeof(long) + userName.size() + ip.size();

}
