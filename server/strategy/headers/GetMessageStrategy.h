#ifndef RING_CATEGORYLISTMESSAGESTRATEGY_H
#define RING_CATEGORYLISTMESSAGESTRATEGY_H


#include "BasicEventStrategy.h"

class GetMessageStrategy : public BasicEventStrategy {
private:
    void sendCategoriesList() const;

    void sendNeighbours(long categoryID) const;

public:
    GetMessageStrategy() : BasicEventStrategy() { }

    GetMessageStrategy(Controller *controller) : BasicEventStrategy(controller) { }

    virtual void serveEvent(SimpleMessage *message) const;
};


#endif //RING_CATEGORYLISTMESSAGESTRATEGY_H
