#include "../headers/Model.h"

shared_ptr<User> Model::createNewUser(const string &userName, int port, string IP) {
    shared_ptr<User> newUser(new User(usersCounter++, port, IP, userName));
    users[newUser->getID()] = newUser;

    return newUser;
}

long Model::createCategory(shared_ptr<User> owner, const string &category_name) {
    for (auto pair : categories) {
        if (pair.second->getName() == category_name)
            return FAILED_CODE;
    }

    auto map_value = shared_ptr<Category>(new Category(categoriesCounter++, owner, category_name));
    categories[map_value->getID()] = map_value;

    return map_value->getID();
}

shared_ptr<User> Model::getUser(const long userID) {
    if (users.find(userID) == users.end())
        return shared_ptr<User>();
    return users.at(userID);
}

shared_ptr<Category> Model::getCategory(const long id) {
    if (categories.find(id) == categories.end())
        return shared_ptr<Category>();
    return categories.at(id);
}

void Model::deleteCategory(shared_ptr<Category> category) {
    categories.at(category->getID()).reset();
    categories.erase(category->getID());
}

void Model::deleteUser(shared_ptr<User> user) {
    users.at(user->getID()).reset();
    categories.erase(user->getID());
}