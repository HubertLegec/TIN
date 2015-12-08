#ifndef RING_USER_H
#define RING_USER_H

#include <iostream>
#include <memory>

using namespace std;

enum Status {
    ONLINE, OFFLINE
};

class User {
private:
    const int id;
    const int port;
    const string IP;
    const string name;
    shared_ptr<User> left_neighbour;
    shared_ptr<User> right_neighbour;
    Status status;

public:
    User(int id, int port, string IP, string name) : id(id), port(port), IP(IP), name(name) {
        status = ONLINE;
    }

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

    Status getStatus() const {
        return status;
    }

    void setStatus(const Status status) {
        this->status = status;
    }

    const shared_ptr<User> getLeftNeighbour() const {
        return left_neighbour;
    }

    const shared_ptr<User> getRightNeighbour() const {
        return right_neighbour;
    }

    void setLeftNeighbour(const shared_ptr<User> neighbour) {
        left_neighbour = neighbour;
    }

    void setRightNeighbour(const shared_ptr<User> neighbour) {
        right_neighbour = neighbour;
    }

    bool operator==(const shared_ptr<User> other) {
        return id == other->id;
    }
};


#endif //RING_USER_H
