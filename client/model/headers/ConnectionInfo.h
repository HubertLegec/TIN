//
// Created by hubert on 18.12.15.
//

#ifndef RING_CONNECTIONINFO_H
#define RING_CONNECTIONINFO_H

#include <string>

class ConnectionInfo {
private:
    std::string name;
    std::string ip;
    int port;
public:
    ConnectionInfo();

    ConnectionInfo(const std::string &ip, int port, const std::string &name);
    ConnectionInfo(const ConnectionInfo& other);
    ConnectionInfo& operator=(const ConnectionInfo& other);

    const std::string &getIP() const;

    void setIP(const std::string &ip);

    const std::string &getName() const;

    void setName(const std::string &name);
    int getPort() const;
    void setPort(int port);
};


#endif //RING_CONNECTIONINFO_H
