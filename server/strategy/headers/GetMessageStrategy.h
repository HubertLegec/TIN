#ifndef RING_CATEGORYLISTMESSAGESTRATEGY_H
#define RING_CATEGORYLISTMESSAGESTRATEGY_H


#include "BasicEventStrategy.h"

class GetMessageStrategy : public BasicEventStrategy {
private:

public:
    GetMessageStrategy() : BasicEventStrategy() { }

    GetMessageStrategy(Controller *controller) : BasicEventStrategy(controller) { }

    virtual void serveEvent(SimpleMessage *message);
};


#endif //RING_CATEGORYLISTMESSAGESTRATEGY_H
