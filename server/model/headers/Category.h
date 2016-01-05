#ifndef RING_CATEGORY_H
#define RING_CATEGORY_H

#include <iostream>
#include <memory>
#include <list>
#include "User.h"
#include "CategoryMember.h"
#include <map>

using namespace std;

class Category {
private:
    bool activated;
    const long id;
    const string name;
    const shared_ptr<User> owner;
    shared_ptr<CategoryMember> members;
    map<long, shared_ptr<User> > unconfirmedUsers;

public:
    Category(long id, const shared_ptr<User> creator, const string new_name) : id(id), owner(creator), name(new_name),
                                                                               activated(true) {
        members.reset(new CategoryMember(owner));
        members->setLeftNeighbour(members);
        members->setRightNeighbour(members);
    }

    ~Category();

    long getID() {
        return id;
    }

    void setActivated() {
        activated = true;
    }

    void setDeactivated() {
        activated = false;
    }

    bool isActivated() {
        return activated;
    }

    void acceptNewUser(long userID);

    void addNewMember(shared_ptr<User> user);

    bool isUnconfirmed(long userID);

    void addMember(shared_ptr<User> user);

    void removeMember(long id);

    void rejectMember(long userID);

    const string &getName() const {
        return name;
    }

    const shared_ptr<User> getOwner() const {
        return owner;
    }

    const shared_ptr<CategoryMember> getMembers() const {
        return members;
    }

    shared_ptr<CategoryMember> findMember(long id);
};


#endif //RING_CATEGORY_H
