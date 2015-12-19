//
// Created by hubert on 19.12.15.
//

#ifndef RING_SHOWINFOEVENT_H
#define RING_SHOWINFOEVENT_H

#include <string>
#include "BasicEvent.h"

class ShowInfoEvent : public BasicEvent {
private:
    std::string info;

public:
    ShowInfoEvent();
    ShowInfoEvent(std::string info);
    void setInfo(std::string);
    std::string getInfo() const;
    std::string toString();
};


#endif //RING_SHOWINFOEVENT_H
