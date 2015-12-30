//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CHOOSEMENUOPTIONEVENT_H
#define RING_CHOOSEMENUOPTIONEVENT_H

#include "BasicEvent.h"

class ChooseMenuOptionEvent : public BasicEvent {

public:
    enum OptionChosen {
        CREATE_USER_ACCOUNT = 'a',
        CREATE_CATEGORY = 'c',
        DELETE_CATEGORY = 'd',
        SHOW_CATEGORY_LIST = 's',
        SIGN_UP_CATEGORY = 'u',
        JOIN_CATEGORY = 'j',
        SIGN_OUT_CATEGORY = 'o',
        LEAVE_CATEGORY = 'l',
        QUIT = 'q',
        REFRESH = 0
    };

    ChooseMenuOptionEvent(const OptionChosen option);
    OptionChosen getOptionChosen();

    virtual std::string toString() const;

    virtual std::string getName() const;

private:
    OptionChosen optionChosen;
};


#endif //RING_CHOOSEMENUOPTIONEVENT_H
