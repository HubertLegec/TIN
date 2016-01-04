#ifndef RING_CATEGORYMEMBER_H
#define RING_CATEGORYMEMBER_H


#include "User.h"

enum Status {
    ONLINE, OFFLINE
};

class CategoryMember {
private:
    const shared_ptr<User> user;
    shared_ptr<CategoryMember> left_neighbour;
    shared_ptr<CategoryMember> right_neighbour;
    Status status;

public:
    CategoryMember(shared_ptr<User> user) : user(user), status(ONLINE) { }

    CategoryMember(shared_ptr<User> user, Status status) : user(user), status(status) { }

    void setStatus(const Status &status) {
        CategoryMember::status = status;
    }

    const shared_ptr<User> &getUser() const {
        return user;
    }

    const Status &getStatus() const {
        return status;
    }

    const shared_ptr<CategoryMember> getLeftNeighbour() const {
        return left_neighbour;
    }

    const shared_ptr<CategoryMember> getRightNeighbour() const {
        return right_neighbour;
    }

    void setLeftNeighbour(const shared_ptr<CategoryMember> neighbour) {
        left_neighbour = neighbour;
    }

    void setRightNeighbour(const shared_ptr<CategoryMember> neighbour) {
        right_neighbour = neighbour;
    }
};


#endif //RING_CATEGORYMEMBER_H
