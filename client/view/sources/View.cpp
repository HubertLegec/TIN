//
// Created by hubert on 15.12.15.
//

#include "../headers/View.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include <iomanip>


View::View(Controller *controller) : controller(controller){

}

void View::showCategoryList(std::map<long, std::string> categories) {
    int idWidth = 21; //Max long type decimal digits number + 1.
    std::cout<<"Category list:"<<std::endl;

    std::cout << std::setiosflags(std::ios::left);

    std::cout<<std::setw(idWidth)<<"id"<<"name"<<std::endl;
    std::cout<<std::setw(idWidth)<<"----"<<"----"<<std::endl;

    for(auto category : categories)
    {
        std::cout<<std::setw(idWidth)<<std::setiosflags(std::ios::left)<<category.first<<category.second<<std::endl;
    }

    std::resetiosflags(std::ios::left);
}

void View::showMainMenu(std::vector<std::string> notificationsList) {

    std::cout<<"Welcome to #RING!"<<std::endl;
    std::cout<<"[c] create category"<<std::endl;
    std::cout<<"[j] join category"<<std::endl;
    std::cout<<"[l] leave category"<<std::endl;
    std::cout<<"[q] quit"<<std::endl;

    if(!notificationsList.empty())
    {
        std::cout<<"-------------"<<std::endl;
        std::cout<<"notifications"<<std::endl;
        std::cout<<"-------------"<<std::endl;

        for(std::string notification : notificationsList )
        {
            std::cout<<notification<<std::endl;
        }
        std::cout<<"-------------"<<std::endl;

    }
    std::string typed;

    ChooseMenuOptionEvent event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::UNDEFINED);

    while(true)
    {
        std::cout<<"choose action:";
        std::cin>>typed;

        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.sync();
        }
        else
        switch(typed[0])
        {
            case 'c': event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::CREATE_CATEGORY);break;
            case 'j': event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::CREATE_CATEGORY);break;
            case 'l': event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::CREATE_CATEGORY);break;
            case 'q':event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::QUIT);break;
        }

        if(event.getOptionChosen()!=ChooseMenuOptionEvent::UNDEFINED)
        {
            controller->getEventsToServe()->push(event);
            break;
        }

        std::cout<<"unrecognized option"<<std::endl;
    }

}

