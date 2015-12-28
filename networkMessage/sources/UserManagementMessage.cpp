#include "../headers/UserManagementMessage.h"

UserManagementMessage::UserManagementMessage(MessageType type, string ip, int port, string login) : SimpleMessage(type,
                                                                                                                   UNDEFINED_ID) {
    this->ip = ip;
    this->port = port;
    this->login = login;

}

UserManagementMessage::UserManagementMessage(long senderID, MessageType type) : SimpleMessage(type, senderID) {
}
