#ifndef RING_MODEL_H
#define RING_MODEL_H

#include <unordered_map>
#include "Category.h"

class Model {
private:
    unordered_map<string, shared_ptr<Category> > categories;

public:
    const unordered_map<string, shared_ptr<Category>> &getCategories() const {
        return categories;
    }

    const shared_ptr<User> getCategoryOwner(string category_name) const;

    const shared_ptr<User> getCategoryMembers(string category_name) const;

    void addCategory(shared_ptr<User> owner, string category_name);

    void addMemberToCategory(shared_ptr<User> member, string category_name);
};


#endif //RING_MODEL_H
