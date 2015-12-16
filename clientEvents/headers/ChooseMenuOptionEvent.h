//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CHOOSEMENUOPTIONEVENT_H
#define RING_CHOOSEMENUOPTIONEVENT_H

#include "BasicEvent.h"

class ChooseMenuOptionEvent : public BasicEvent {


public:
    ChooseMenuOptionEvent();
    std::string toString();
};


#endif //RING_CHOOSEMENUOPTIONEVENT_H
