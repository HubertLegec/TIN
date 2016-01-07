#include "../headers/Category.h"

void Category::addMember(shared_ptr<User> user) {
    shared_ptr<CategoryMember> newMember(new CategoryMember(user));
    if (members) {
        members = newMember;
        newMember->setLeftNeighbour(members);
        newMember->setRightNeighbour(members);
        return;
    }

    auto categoryMember = members;
    do {
        if (categoryMember->getUser()->getID() == user->getID())
            throw runtime_error("Member, you wanted to add, already exists in the category!");
    } while ((categoryMember = categoryMember->getLeftNeighbour()) != members);

    auto first = members;
    auto last = members->getLeftNeighbour();

    first->setLeftNeighbour(newMember);
    newMember->setRightNeighbour(first);

    last->setRightNeighbour(newMember);
    newMember->setLeftNeighbour(last);
}

shared_ptr<CategoryMember> Category::findMember(long id) {
    if (!members)
        return members;

    auto categoryMember = members;
    do {
        if (categoryMember->getUser()->getID() == id)
            return categoryMember;
    } while ((categoryMember = categoryMember->getLeftNeighbour()) != members);

    return shared_ptr<CategoryMember>();
}

void Category::removeMember(long id) {
    auto member = findMember(id);
    if (member->getLeftNeighbour() == member &&
        member->getRightNeighbour() == member) {
        members = shared_ptr<CategoryMember>();

        return;
    }

    auto left_neighbour = member->getLeftNeighbour();
    auto right_neighbour = member->getRightNeighbour();

    left_neighbour->setRightNeighbour(right_neighbour);
    right_neighbour->setLeftNeighbour(left_neighbour);

    if (members == member)
        members = member->getLeftNeighbour();
}

void Category::addNewMember(shared_ptr<User> user) {
    if (isUnconfirmed(user->getID()) || findMember(user->getID()))
        throw runtime_error("Member, you wanted to add, already exists in the category!");

    unconfirmedUsers[user->getID()] = user;
}

bool Category::isUnconfirmed(long userID) {
    return unconfirmedUsers.find(userID) != unconfirmedUsers.end();
}

void Category::rejectMember(long userID) {
    unconfirmedUsers.erase(userID);
}

void Category::acceptNewUser(long userID) {
    addMember(unconfirmedUsers.at(userID));
    unconfirmedUsers.erase(userID);
}

Category::~Category() {
    if (members)
        members->setLeftNeighbour(shared_ptr<CategoryMember>());
}
