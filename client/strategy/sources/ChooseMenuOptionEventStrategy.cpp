//
// Created by hubert.legec on 2015-12-15.
//

#include "../headers/ChooseMenuOptionEventStrategy.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"

ChooseMenuOptionEventStrategy::ChooseMenuOptionEventStrategy() : BasicEventStrategy(){ }

ChooseMenuOptionEventStrategy::ChooseMenuOptionEventStrategy(Controller* controller) : BasicEventStrategy(controller) { }

void ChooseMenuOptionEventStrategy::serveEvent(BasicEvent event) {
    ChooseMenuOptionEvent& chooseMenuOptionEvent = dynamic_cast<ChooseMenuOptionEvent&>(event);
    //TODO
}