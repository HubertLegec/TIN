//
// Created by hubert on 18.12.15.
//

#include "../headers/ConnectionInfo.h"

ConnectionInfo::ConnectionInfo() { }

ConnectionInfo::ConnectionInfo(std::string ip, int port, std::string name) : ip(ip), port(port), name(name){ }

ConnectionInfo::ConnectionInfo(const ConnectionInfo &other) {
    this->ip = other.ip;
    this->port = other.port;
    this->name = other.name;
}

ConnectionInfo& ConnectionInfo::operator=(const ConnectionInfo &other) {
    this->ip = other.ip;
    this->port = other.port;
    this->name = other.name;
    return *this;
}

std::string ConnectionInfo::getIP() const {
    return ip;
}

int ConnectionInfo::getPort() const {
    return port;
}

std::string ConnectionInfo::getName() const {
    return name;
}

void ConnectionInfo::setIP(std::string ip) {
    this->ip = ip;
}

void ConnectionInfo::setPort(int port) {
    this->port = port;
}

void ConnectionInfo::setName(std::string name) {
    this->name = name;
}