#ifndef RING_CATEGORYMANAGEMENTSTRATEGY_H
#define RING_CATEGORYMANAGEMENTSTRATEGY_H

#include "BasicEventStrategy.h"
#include "../../model/headers/CategoryMember.h"

#include <string>

using namespace std;

class CategoryManagementStrategy : public BasicEventStrategy {
private:

public:
    CategoryManagementStrategy() : BasicEventStrategy() { }

    CategoryManagementStrategy(Controller *controller) : BasicEventStrategy(controller) { }

    virtual void serveEvent(SimpleMessage *message) const;
};


#endif //RING_CATEGORYMANAGEMENTSTRATEGY_H
