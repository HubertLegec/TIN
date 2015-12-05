#ifndef RING_CATEGORY_H
#define RING_CATEGORY_H

#include <iostream>
#include <memory>
#include <vector>
#include "User.h"

using namespace std;

class Category {
private:
    const string name;
    const shared_ptr<User> owner;
    vector<shared_ptr<User> > members;

public:
    Category(const shared_ptr<User> creator, const string new_name) : owner(creator), name(new_name) {
    }

    void addMember(const shared_ptr<User> member);

    const string &getName() const {
        return name;
    }

    const shared_ptr<User> getOwner() const {
        return owner;
    }

    const vector<shared_ptr<User>> &getMembers() const {
        return members;
    }
};


#endif //RING_CATEGORY_H
