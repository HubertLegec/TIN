//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CHOOSEMENUOPTIONEVENT_H
#define RING_CHOOSEMENUOPTIONEVENT_H

#include "BasicEvent.h"

class ChooseMenuOptionEvent : public BasicEvent {

public:
    enum OptionChosen {
        CREATE_CATEGORY = 'c',
        DELETE_CATEGORY = 'd',
        SHOW_CATEGORY_LIST = 's',
        REGISTER_IN_CATEGORY = 'r',
        JOIN_CATEGORY = 'j',
        VISIT_CATEGORY = 'v',
        LEAVE_CATEGORY = 'l',
        QUIT = 'q',
        REFRESH  = 0 };

    ChooseMenuOptionEvent(OptionChosen option);
    OptionChosen getOptionChosen();
    std::string toString();

private:
    OptionChosen optionChosen;
};


#endif //RING_CHOOSEMENUOPTIONEVENT_H
