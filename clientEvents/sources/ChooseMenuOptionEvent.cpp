//
// Created by hubert.legec on 2015-12-15.
//

#include "../headers/ChooseMenuOptionEvent.h"

ChooseMenuOptionEvent::ChooseMenuOptionEvent(OptionChosen option) : BasicEvent(), optionChosen(option) { }

ChooseMenuOptionEvent::OptionChosen ChooseMenuOptionEvent::getOptionChosen()
{
    return optionChosen;
}

std::string ChooseMenuOptionEvent::toString() {
    //TODO
    return "ChooseMenuOptionEvent";
}