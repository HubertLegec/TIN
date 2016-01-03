#ifndef RING_ERRORMESSAGESTRATEGY_H
#define RING_ERRORMESSAGESTRATEGY_H

#include "BasicEventStrategy.h"

class ErrorMessageStrategy : public BasicEventStrategy {
public:
    ErrorMessageStrategy() : ErrorMessageStrategy() { }

    ErrorMessageStrategy(Controller *controller) : ErrorMessageStrategy(controller) { }

    virtual void serveEvent(SimpleMessage *message) const;
};


#endif //RING_ERRORMESSAGESTRATEGY_H
