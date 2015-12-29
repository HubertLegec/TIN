//
// Created by hubert.legec on 2015-12-15.
//

#include "../headers/ChooseMenuOptionEvent.h"

ChooseMenuOptionEvent::ChooseMenuOptionEvent(const OptionChosen option) : BasicEvent(), optionChosen(option) { }

ChooseMenuOptionEvent::OptionChosen ChooseMenuOptionEvent::getOptionChosen()
{
    return optionChosen;
}

std::string ChooseMenuOptionEvent::toString() {
    //TODO
    return "ChooseMenuOptionEvent";
}

std::string ChooseMenuOptionEvent::getName() {
    return "CHOOSE_MENU_OPTION_EVENT";
}