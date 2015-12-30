#include "../headers/Model.h"

const shared_ptr<User> Model::getCategoryOwner(const long categoryID) {
    return categories.at(categoryID)->getOwner();
}

const shared_ptr<CategoryMember> Model::getCategoryMembers(const long categoryID) {
    return categories.at(categoryID)->getMembers();
}

shared_ptr<User> Model::createNewUser(const string &userName, int port, string IP) {
    shared_ptr<User> newUser(new User(usersCounter++, port, IP, userName));
    users[newUser->getID()] = newUser;

    return newUser;
}

void Model::addCategory(shared_ptr<User> owner, const string &category_name) {
    for (auto pair : categories) {
        if (pair.second->getName() == category_name)
            throw runtime_error("Category, you wanted to add, already exists!");
    }

    auto map_value = shared_ptr<Category>(new Category(categoriesCounter++, owner, category_name));
    categories[map_value->getID()] = map_value;
}

shared_ptr<User> Model::getUser(const long userID) {
    return users.at(userID);
}

shared_ptr<Category> Model::getCategory(const long id) {
    return categories.at(id);
}

void Model::createCategory(long ownerID, const string &category_name) {
    auto owner = users.at(ownerID);
    addCategory(owner, category_name);
}

void Model::addMemberToCategory(shared_ptr<User> member, long categoryID) {
    categories.at(categoryID)->addMember(member);
}

void Model::destroyCategory(const long categoryID) {
    categories.at(categoryID).reset();
    categories.erase(categoryID);
}

void Model::deleteUser(const long userID) {
    for (auto pair : categories) {
        auto member = pair.second->findMember(userID);
        if (member) {
            pair.second->removeMember(userID);
        }
    }

    users.at(userID).reset();
    categories.erase(userID);
}