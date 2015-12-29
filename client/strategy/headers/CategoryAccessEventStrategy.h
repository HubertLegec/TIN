//
// Created by hubert on 29.12.15.
//

#ifndef RING_CATEGORYACCESSEVENTSTRATEGY_H
#define RING_CATEGORYACCESSEVENTSTRATEGY_H

#include "BasicEventStrategy.h"

class CategoryAccessEventStrategy : public BasicEventStrategy {
private:

public:
    CategoryAccessEventStrategy();

    CategoryAccessEventStrategy(Controller *controller);

    virtual void serveEvent(BasicEvent *event) override;
};


#endif //RING_CATEGORYACCESSEVENTSTRATEGY_H
