#ifndef RING_CATEGORYLISTMESSAGESTRATEGY_H
#define RING_CATEGORYLISTMESSAGESTRATEGY_H


#include "BasicEventStrategy.h"

class CategoryListMessageStrategy : public BasicEventStrategy {
private:

public:
    CategoryListMessageStrategy() : BasicEventStrategy() { }

    CategoryListMessageStrategy(Controller *controller) : BasicEventStrategy(controller) { }

    virtual void serveEvent(SimpleMessage *message);
};


#endif //RING_CATEGORYLISTMESSAGESTRATEGY_H
