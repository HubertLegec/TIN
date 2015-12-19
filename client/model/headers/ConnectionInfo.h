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
    ConnectionInfo(std::string ip, int port, std::string name);
    ConnectionInfo(const ConnectionInfo& other);
    ConnectionInfo& operator=(const ConnectionInfo& other);
    std::string getIP() const;
    void setIP(std::string ip);
    std::string getName() const;
    void setName(std::string name);
    int getPort() const;
    void setPort(int port);
};


#endif //RING_CONNECTIONINFO_H
