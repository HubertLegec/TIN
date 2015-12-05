#include "../headers/Category.h"

void Category::addMember(const shared_ptr<User> member) {
    if(*owner == member)
        throw runtime_error("Member, you wanted to add, is an owner of the category!");

    for(auto user : members) {
        if(*user == member)
            throw runtime_error("Member, you wanted to add, already exists in the category!");
    }

    members.push_back(member);
}
