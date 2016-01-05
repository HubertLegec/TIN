//
// Created by hubert on 18.12.15.
//

#include "../headers/ConnectionInfo.h"

using namespace std;

ConnectionInfo::ConnectionInfo() { }

ConnectionInfo::ConnectionInfo(const string &ip, int port, const string &name) : ip(ip), port(port), name(name) {

}

ConnectionInfo::ConnectionInfo(const ConnectionInfo &other) {
    this->ip = other.ip;
    this->port = other.port;
    this->name = other.name;
}

ConnectionInfo &ConnectionInfo::operator=(const ConnectionInfo &other) {
    this->ip = other.ip;
    this->port = other.port;
    this->name = other.name;
    return *this;
}

const string &ConnectionInfo::getIP() const {
    return ip;
}

int ConnectionInfo::getPort() const {
    return port;
}

const string &ConnectionInfo::getName() const {
    return name;
}

void ConnectionInfo::setIP(const string &ip) {
    this->ip = ip;
}

void ConnectionInfo::setPort(int port) {
    this->port = port;
}

void ConnectionInfo::setName(const string &name) {
    this->name = name;
}