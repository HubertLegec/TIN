#include "../headers/Category.h"

void Category::addMember(shared_ptr<User> member) {
    if (*owner == member)
        throw runtime_error("Member, you wanted to add, is an owner of the category!");

    for (auto user = members; user->getRightNeighbour() != members; user = user->getRightNeighbour()) {
        if (*user == member)
            throw runtime_error("Member, you wanted to add, already exists in the category!");
    }

    auto first = members;
    auto last = members->getLeftNeighbour();

    first->setLeftNeighbour(member);
    member->setRightNeighbour(first);

    last->setRightNeighbour(member);
    member->setLeftNeighbour(last);
}

shared_ptr<User> Category::findUser(int id) {
    for (auto user = members; user->getRightNeighbour() != members; user = user->getRightNeighbour()) {
        if (user->getID() == id) {
            return user;
        }
    }

    throw out_of_range("Couldn't find specified user in the category!");
}

void Category::removeMember(int id) {
    auto member = findUser(id);

    auto left_neighbour = member->getLeftNeighbour();
    auto right_neighbour = member->getRightNeighbour();

    left_neighbour->setRightNeighbour(right_neighbour);
    right_neighbour->setLeftNeighbour(left_neighbour);
}
