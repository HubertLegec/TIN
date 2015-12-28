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

    ChooseMenuOptionEvent::OptionChosen event = ChooseMenuOptionEvent::REFRESH;


    std::cout << ">";
    std::cin.clear();
    std::cin.sync();
    typed = std::cin.get();

    switch (typed) {
        case 'c':
            event = ChooseMenuOptionEvent::CREATE_CATEGORY;
            break;
        case 'd':
            event = ChooseMenuOptionEvent::DELETE_CATEGORY;
            break;
        case 's':
            event = ChooseMenuOptionEvent::SHOW_CATEGORY_LIST;
            break;
        case 'r':
            event = ChooseMenuOptionEvent::REGISTER_IN_CATEGORY;
            break;
        case 'j':
            event = ChooseMenuOptionEvent::JOIN_CATEGORY;
            break;
        case 'v':
            event = ChooseMenuOptionEvent::VISIT_CATEGORY;
            break;
        case 'l':
            event = ChooseMenuOptionEvent::LEAVE_CATEGORY;
            break;
        case 'q':
            event = ChooseMenuOptionEvent::QUIT;
            break;
    }

    controller->getEventsToServe()->push(std::shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(event)));

}

void View::showCreateCategorySubMenu() {
    std::string categoryName;
    std::string userName;
    std::string userPassword;


    std::cout << "Creating category:" << std::endl;

    readCategoryAccessData(categoryName,userName,userPassword);

    std::cout << "Category name: " << categoryName << std::endl;
    std::cout << "User name: " << userName << std::endl;
    std::cout << "Create category? [y/n] " << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::CREATE_CATEGORY, categoryName, userName, userPassword)));
    }
}

void View::showDeleteCategorySubMenu()
{
    std::string categoryName,userName,userPassword;
    std::cout<<"Deleting catefory:"<<std::endl;

    readCategoryAccessData(categoryName,userName,userPassword,false);

    std::cout << "Delete category "<<categoryName<<"  [y/n]?" << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::DELETE_CATEGORY, categoryName, userName, userPassword)));
    }
}

void View::showRegisterInCategorySubMenu()
{
    std::string categoryName,userName,userPassword;
    std::cout<<"Registering in catefory:"<<std::endl;

    readCategoryAccessData(categoryName,userName,userPassword);

    std::cout << "Category name: " << categoryName << std::endl;
    std::cout << "User name: " << userName << std::endl;
    std::cout << "Register in category? [y/n] " << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::REGISTER_IN_CATEGORY, categoryName, userName,
                                        userPassword)));
    }
}
void View::showJoinCategorySubMenu()
{
    std::string categoryName,userName,userPassword;
    std::cout<<"Joining catefory:"<<std::endl;

    readCategoryAccessData(categoryName,userName,userPassword,false);

    std::cout << "Join category "<<categoryName<<"  [y/n]?" << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::JOIN_CATEGORY, categoryName, userName, userPassword)));
    }
}
void View::showLeaveCategorySubMenu()
{
    std::string categoryName;
    std::cout<<"Leaving category:"<<std::endl;

    std::cout<<"Type category name:";
    std::cin>>categoryName;

    std::cout << "Leave category "<<categoryName<<"  [y/n]?" << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::LEAVE_CATEGORY, categoryName)));
    }
}

void View::showVisitCategorySubMenu()
{
    std::string categoryName;
    std::cout<<"Visiting category:"<<std::endl;

    std::cout<<"Type category name:";
    std::cin>>categoryName;

    std::cout << "Visit category "<<categoryName<<"  [y/n]?" << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::VISIT_CATEGORY, categoryName)));
    }
}

void View::readCategoryAccessData(std::string & categoryName, std::string & userName, std::string & userPassword, bool confirmPassword)
{
    std::string passwordConfirmation;

    std::cout<<"Type category name:";
    std::cin>>categoryName;

    std::cout<<"Type user name:";
    std::cin>>userName;

    while(true) {
        std::cout << "Type your password:";
        std::cin >> userPassword;

        if(!confirmPassword)
            break;

        std::cout << "Confirm Password:";
        std::cin >> passwordConfirmation;

        if (userPassword == passwordConfirmation)
            break;
        else
            std::cout << "Passwords don't match!" << std::endl;
    }
}




