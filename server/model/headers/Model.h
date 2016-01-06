#ifndef RING_MODEL_H
#define RING_MODEL_H

#include <unordered_map>
#include "Category.h"

enum {
    FAILED_CODE = -3,
    UNKNOWN_CODE = -4
};

class Model {
private:
    long categoriesCounter;
    long usersCounter;
    unordered_map<long, shared_ptr<Category> > categories;
    unordered_map<long, shared_ptr<User> > users;

public:
    Model() : categoriesCounter(0), usersCounter(1) { }

    const unordered_map<long, shared_ptr<Category>> &getCategories() const {
        return categories;
    }

    shared_ptr<User> createNewUser(const std::string &userName, int port, string IP);

    long createCategory(shared_ptr<User> owner, const string &category_name);

    void deleteUser(shared_ptr<User> user);

    shared_ptr<User> getUser(const long userID);

    shared_ptr<Category> getCategory(const long id);

    void deleteCategory(shared_ptr<Category> category);
};


#endif //RING_MODEL_H
