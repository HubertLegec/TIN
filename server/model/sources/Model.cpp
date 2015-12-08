#include "../headers/Model.h"

const shared_ptr<User> Model::getCategoryOwner(string category_name) const {
    return categories.at(category_name)->getOwner();
}

const shared_ptr<User> Model::getCategoryMembers(string category_name) const {
    return categories.at(category_name)->getMembers();
}

void Model::addCategory(shared_ptr<User> owner, string category_name) {
    if (categories.count(category_name) > 0)
        throw out_of_range("Category, you wanted to add, already exists!");

    auto map_value = shared_ptr<Category>(new Category(owner, category_name));
    auto pair_to_add = make_pair(category_name.c_str(), map_value);

    categories.insert(pair_to_add);
}

void Model::addMemberToCategory(shared_ptr<User> member, string category_name) {
    categories.at(category_name)->addMember(member);
}
