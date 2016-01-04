#include "../headers/Category.h"

void Category::addMember(shared_ptr<User> member) {
    if (*owner == member)
        throw runtime_error("Member, you wanted to add, is an owner of the category!");

    for (auto categoryMember = members;
         categoryMember->getRightNeighbour() != members; categoryMember = categoryMember->getRightNeighbour()) {
        if (*categoryMember->getUser() == member)
            throw runtime_error("Member, you wanted to add, already exists in the category!");
    }

    shared_ptr<CategoryMember> newMember(new CategoryMember(member));
    auto first = members;
    auto last = members->getLeftNeighbour();

    first->setLeftNeighbour(newMember);
    newMember->setRightNeighbour(first);

    last->setRightNeighbour(newMember);
    newMember->setLeftNeighbour(last);
}

shared_ptr<CategoryMember> Category::findMember(long id) {
    if(members->getUser()->getID() == id)
        return members;

    for (auto categoryMember = members;
         categoryMember->getRightNeighbour() != members; categoryMember = categoryMember->getRightNeighbour()) {
        if (categoryMember->getUser()->getID() == id) {
            return categoryMember;
        }
    }

//    return shared_ptr<CategoryMember>();
    throw out_of_range("Couldn't find specified user in the category!");
}

void Category::removeMember(long id) {
    auto member = findMember(id);

    auto left_neighbour = member->getLeftNeighbour();
    auto right_neighbour = member->getRightNeighbour();

    left_neighbour->setRightNeighbour(right_neighbour);
    right_neighbour->setLeftNeighbour(left_neighbour);
}
