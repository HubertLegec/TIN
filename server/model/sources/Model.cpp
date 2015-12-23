#include "../headers/Model.h"

const shared_ptr<User> Model::getCategoryOwner(const long categoryID) {
    return categories[categoryID]->getOwner();
}

const shared_ptr<CategoryMember> Model::getCategoryMembers(const long categoryID) {
    return categories[categoryID]->getMembers();
}

shared_ptr<User> Model::createNewUser(const string &userName, int port, string IP) {
    shared_ptr<User> newUser(new User(usersCounter++, port, IP, userName));

    auto pair_to_add = make_pair(newUser->getID(), newUser);
    users.insert(pair_to_add);

    return newUser;
}

void Model::addCategory(shared_ptr<User> owner, const string &category_name) {
    for (auto pair : categories) {
        if (pair.second->getName() == category_name)
            throw runtime_error("Category, you wanted to add, already exists!");
    }

    auto map_value = shared_ptr<Category>(new Category(categoriesCounter++, owner, category_name));
    auto pair_to_add = make_pair(map_value->getID(), map_value);

    categories.insert(pair_to_add);
}

shared_ptr<User> Model::getUser(const long userID) {
    return users[userID];
}

shared_ptr<Category> Model::getCategory(const long id) {
    return categories[id];
}

void Model::createCategory(long ownerID, const string &category_name) {
    auto owner = users[ownerID];
    addCategory(owner, category_name);
}

void Model::addMemberToCategory(shared_ptr<User> member, long categoryID) {
    categories[categoryID]->addMember(member);
}

void Model::destroyCategory(const long id) {
    // TODO
    // Send to members message that they leave category

    categories[id].reset();
}