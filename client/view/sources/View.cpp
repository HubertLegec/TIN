//
// Created by hubert on 15.12.15.
//

#include "../headers/View.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../../clientEvents/headers/CategoryAccessEvent.h"
#include <iomanip>


View::View(Controller *controller) : controller(controller) {

}

void View::showCategoryList(std::map<long, std::string> categories) {
    int idWidth = 21; //Max long type decimal digits number + 1.
    std::cout << "Category list:" << std::endl;

    std::cout << std::setiosflags(std::ios::left);

    std::cout << std::setw(idWidth) << "id" << "name" << std::endl;
    std::cout << std::setw(idWidth) << "----" << "----" << std::endl;

    for (auto category : categories) {
        std::cout << std::setw(idWidth) << std::setiosflags(std::ios::left) << category.first << category.second <<
        std::endl;
    }

    std::resetiosflags(std::ios::left);
}

void showCreateCategorySubMenu()
{
    std::cin.clear();
    std::cin.sync();

    std::string userName;
    std::string userPassword;
    std::string categoryName;

    std::cout<<"Enter category name:";
}


void View::showMainMenu(std::vector<std::string> notificationsList) {

    std::cout << "Welcome to #RING!" << std::endl;
    std::cout << "choose action:"<<std::endl;
    std::cout << "[c] create category" << std::endl;
    std::cout << "[d] delete category" << std::endl;
    std::cout << "[s] show category list" << std::endl;
    std::cout << "[r] register in category" << std::endl;
    std::cout << "[j] join category" << std::endl;
    std::cout << "[v] visit category" << std::endl;
    std::cout << "[l] leave category" << std::endl;
    std::cout << "[q] quit" << std::endl;

    if (!notificationsList.empty()) {
        std::cout << "-------------" << std::endl;
        std::cout << "notifications" << std::endl;
        std::cout << "-------------" << std::endl;

        for (std::string notification : notificationsList) {
            std::cout << notification << std::endl;
        }
        std::cout << "-------------" << std::endl;

    }
    std::cout << "[other] refresh" << std::endl;

    char typed;

    ChooseMenuOptionEvent event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH);


    std::cout << ">";
    std::cin.clear();
    std::cin.sync();
    typed = std::cin.get();

    switch (typed) {
        case 'c':
            event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::CREATE_CATEGORY);
            break;
        case 'd':
            event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::DELETE_CATEGORY);
            break;
        case 's':
            event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::SHOW_CATEGORY_LIST);
            break;
        case 'r':
            event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::REGISTER_IN_CATEGORY);
            break;
        case 'j':
            event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::JOIN_CATEGORY);
            break;
        case 'v':
            event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::VISIT_CATEGORY);
            break;
        case 'l':
            event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::LEAVE_CATEGORY);
            break;
        case 'q':
            event = ChooseMenuOptionEvent(ChooseMenuOptionEvent::QUIT);
            break;
    }

    controller->getEventsToServe()->push(event);

}

void View::showCreateCategorySubMenu()
{
    std::string categoryName;
    std::string userName;
    std::string userPassword;
    std::string passwordConfirmation;


    std::cout<<"Creating category:"<<std::endl;

    std::cout<<"Type category name:";
    std::cin>>categoryName;

    std::cout<<"Type user name:";
    std::cin>>userName;

    while(true)
    {
        std::cout<<"Type your password:";
        std::cin>>userPassword;

        std::cout<<"Confirm Password:";
        std::cin>>passwordConfirmation;

        if(userPassword==passwordConfirmation)
            break;
        else
            std::cout<<"Passwords don't match!"<<std::endl;
    }

    std::cout<<"Category name: "<<categoryName<<std::endl;
    std::cout<<"User name: "<<userName<<std::endl;
    std::cout<<"Create category? [y/n] "<<std::endl;

    char decision;
    std::cin>>decision;
    if(decision == 'y')
    {
        CategoryAccessEvent event(CategoryAccessEvent::CREATE_CATEGORY,categoryName,userName,userPassword);
        controller->getEventsToServe()->push(event);
        std::cout<<"Category created."<<std::endl;
    }
}


