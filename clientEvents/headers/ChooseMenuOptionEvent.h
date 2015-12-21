//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CHOOSEMENUOPTIONEVENT_H
#define RING_CHOOSEMENUOPTIONEVENT_H

#include "BasicEvent.h"

class ChooseMenuOptionEvent : public BasicEvent {

public:
    enum OptionChosen {CREATE_CATEGORY = 'c', JOIN_CATEGORY = 'j', LEAVE_CATEGORY = 'l', QUIT = 'q', UNDEFINED  = 0 };

    ChooseMenuOptionEvent(OptionChosen option);
    OptionChosen getOptionChosen();
    std::string toString();

private:
    OptionChosen optionChosen;
};


#endif //RING_CHOOSEMENUOPTIONEVENT_H
