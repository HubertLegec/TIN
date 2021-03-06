#include "../headers/UserManagementMessage.h"

using namespace std;

UserManagementMessage::UserManagementMessage() : SimpleMessage() {
    this->size = SimpleMessage::getMessageSize() + sizeof(int) + 2 * sizeof(long);
};

UserManagementMessage::UserManagementMessage(MessageType type, const string &ip, int port, const string &login)
        : SimpleMessage(type,
                        UNDEFINED_ID) {
    this->ip = ip;
    this->port = port;
    this->userName = login;
    this->size = SimpleMessage::getMessageSize() + sizeof(int) + 2 * sizeof(long) + userName.size() + ip.size();
}

UserManagementMessage::UserManagementMessage(long senderID, MessageType type) : SimpleMessage(type, senderID) {
    this->size = SimpleMessage::getMessageSize() + sizeof(int) + 2 * sizeof(long) + userName.size() + ip.size();
}

int UserManagementMessage::getPort() const {
    return port;
}

const string &UserManagementMessage::getIp() const {
    return ip;
}

const string &UserManagementMessage::getUserName() const {
    return userName;
}

string UserManagementMessage::toString() const {
    std::stringstream ss;
    ss << "UserManagementMessage[type:" << type << "; ";
    ss << "size:" << size << "; ";
    ss << "senderID" << senderID << ";\n";
    ss << "userName:" << userName << "; ";
    ss << "ip:" << ip << "; ";
    ss << "port:" << port << "]";
    return ss.str();
}
