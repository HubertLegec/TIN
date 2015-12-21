#ifndef RING_USER_H
#define RING_USER_H

#include <iostream>
#include <memory>

using namespace std;

class User {
private:
    const long id;
    const int port;
    const string IP;
    const string name;

public:
    User(long id, int port, string IP, string name) : id(id), port(port), IP(IP), name(name) { }

    int getID() const {
        return id;
    }

    int getPort() const {
        return port;
    }

    const string &getIP() const {
        return IP;
    }

    const string &getName() const {
        return name;
    }

    bool operator==(const shared_ptr<User> other) {
        return id == other->id;
    }
};


#endif //RING_USER_H
