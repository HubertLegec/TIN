//
// Created by hubert.legec on 2015-12-15.
//

#include <sstream>
#include "../headers/ChooseMenuOptionEvent.h"

using namespace std;

ChooseMenuOptionEvent::ChooseMenuOptionEvent(const OptionChosen option) : BasicEvent(), optionChosen(option) { }

ChooseMenuOptionEvent::OptionChosen ChooseMenuOptionEvent::getOptionChosen() {
    return optionChosen;
}

string ChooseMenuOptionEvent::toString() const {
    stringstream ss;
    ss << "ChooseMenuOptionEvent[optionChoosen:" << optionChosen << "]";
    return ss.str();
}

string ChooseMenuOptionEvent::getName() const {
    return "CHOOSE_MENU_OPTION_EVENT";
}