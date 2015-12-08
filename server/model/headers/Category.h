#ifndef RING_CATEGORY_H
#define RING_CATEGORY_H

#include <iostream>
#include <memory>
#include <list>
#include "User.h"

using namespace std;

class Category {
private:
    const string name;
    const shared_ptr<User> owner;
    shared_ptr<User> members;

    shared_ptr<User> findUser(int id);

public:
    Category(const shared_ptr<User> creator, const string new_name) : owner(creator), name(new_name) {
        owner->setLeftNeighbour(owner);
        owner->setRightNeighbour(owner);
        members = owner;
    }

    // TODO fabryka to tworzenia members...
    void addMember(shared_ptr<User> member);

    void removeMember(int id);

    const string &getName() const {
        return name;
    }

    const shared_ptr<User> getOwner() const {
        return owner;
    }

    const shared_ptr<User> getMembers() const {
        return members;
    }
};


#endif //RING_CATEGORY_H
