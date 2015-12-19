//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CHOOSEMENUOPTIONEVENTSTRATEGY_H
#define RING_CHOOSEMENUOPTIONEVENTSTRATEGY_H

#include "BasicEventStrategy.h"


class ChooseMenuOptionEventStrategy : public BasicEventStrategy {


public:
    ChooseMenuOptionEventStrategy();
    ChooseMenuOptionEventStrategy(Controller* controller);
    void serveEvent(BasicEvent event);
};


#endif //RING_CHOOSEMENUOPTIONEVENTSTRATEGY_H
