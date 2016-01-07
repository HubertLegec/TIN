#ifndef RING_CATEGORYMANAGEMENTSTRATEGY_H
#define RING_CATEGORYMANAGEMENTSTRATEGY_H

#include "BasicEventStrategy.h"
#include "../../model/headers/CategoryMember.h"

#include <string>

using namespace std;

class CategoryManagementStrategy : public BasicEventStrategy {
private:
    void createCategory(const string &categoryName) const;

    void destroyCategory(long categoryID) const;

    void joinCategory(long categoryID) const;

    void leaveCategory(long categoryID) const;

    void activateCategory(long categoryID) const;

    void deactivateCategory(long categoryID) const;

    void acceptMember(long categoryID, long memberID) const;

    void rejectMember(long categoryID, long memberID) const;

public:
    CategoryManagementStrategy() : BasicEventStrategy() { }

    CategoryManagementStrategy(Controller *controller) : BasicEventStrategy(controller) { }

    virtual void serveEvent(SimpleMessage *message) const;
};


#endif //RING_CATEGORYMANAGEMENTSTRATEGY_H
