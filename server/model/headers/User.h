#ifndef RING_USER_H
#define RING_USER_H

#include <iostream>
#include <memory>

using namespace std;

class User {
private:
    const int port;
    const string IP;
    const string name;

public:
    User(int port, string IP, string name) : port(port), IP(IP), name(name) {
    }

    const int getPort() const {
        return port;
    }

    const string &getIP() const {
        return IP;
    }

    const string &getName() const {
        return name;
    }

    bool operator==(const shared_ptr<User> other) {
        return name == other->name;
    }
};


#endif //RING_USER_H
