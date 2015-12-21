#ifndef RING_CATEGORYMANAGEMENTSTRATEGY_H
#define RING_CATEGORYMANAGEMENTSTRATEGY_H


#include "BasicEventStrategy.h"

class CategoryManagementStrategy : BasicEventStrategy {
private:

public:
    CategoryManagementStrategy() : BasicEventStrategy() { }

    CategoryManagementStrategy(Controller *controller) : BasicEventStrategy(controller) { }

    virtual void serveEvent(SimpleMessage *message);
};


#endif //RING_CATEGORYMANAGEMENTSTRATEGY_H
