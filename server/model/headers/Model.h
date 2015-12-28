#ifndef RING_MODEL_H
#define RING_MODEL_H

#include <unordered_map>
#include "Category.h"

class Model {
private:
    long categoriesCounter;
    long usersCounter;
    unordered_map<long, shared_ptr<Category> > categories;
    unordered_map<long, shared_ptr<User> > users;

public:
    Model() : categoriesCounter(0), usersCounter(0) { }

    const unordered_map<long, shared_ptr<Category>> &getCategories() const {
        return categories;
    }

    const shared_ptr<User> getCategoryOwner(const long categoryID);

    const shared_ptr<CategoryMember> getCategoryMembers(const long categoryID);

    shared_ptr<User> createNewUser(const std::string &userName, int port, string IP);

    void addCategory(shared_ptr<User> owner, const string &category_name);

    void deleteUser(const long userID);

    shared_ptr<User> getUser(const long userID);

    shared_ptr<Category> getCategory(const long id);

    void createCategory(long ownerID, const string &category_name);

    void destroyCategory(const long id);

    void addMemberToCategory(shared_ptr<User> member, const long categoryID);
};


#endif //RING_MODEL_H
