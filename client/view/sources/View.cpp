//
// Created by hubert on 15.12.15.
//

#include "../headers/View.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../../clientEvents/headers/CategoryAccessEvent.h"
#include <iomanip>

View::View(Controller * controller) : threadData(controller),viewThread() {
}

View::~View() { pthread_join(viewThread,NULL); }


void View::showCategoryList(std::map<long, std::string> categories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = categories;
    pthread_create(&viewThread, NULL, showCreateCategorySubMenuThread, &threadData);
}


void View::showMainMenu(std::vector<std::string> notificationsList) {

    pthread_join(viewThread, NULL);
    *threadData.notifications = notificationsList;
    pthread_create(&viewThread, NULL, showMainMenuThread, &threadData);

}

void View::showCreateCategorySubMenu() {
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showCreateCategorySubMenuThread, &threadData);
}

void View::showDeleteCategorySubMenu()
{
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showDeleteCategorySubMenuThread, &threadData);
}

void View::showRegisterInCategorySubMenu()
{
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showRegisterInCategorySubMenuThread,&threadData);
}
void View::showJoinCategorySubMenu()
{
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showJoinCategorySubMenuThread,&threadData);
}
void View::showLeaveCategorySubMenu()
{
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showLeaveCategorySubMenuThread,&threadData);
}

void View::showVisitCategorySubMenu()
{
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showVisitCategorySubMenuThread,&threadData);
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

bool View::getUserConfirmation() {
    std::cout << std::endl << "Are you sure (y/n)?" << std::endl;
    char decision;
    std::cin >> decision;
    std::cout << std::endl;
    if (decision == 'y') {
        return true;
    }
    else {
        return false;
    }
}


void* View::showCategoryListThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    int idWidth = 21; //Max long type decimal digits number + 1.
    std::cout << "Category list:" << std::endl;

    std::cout << std::setiosflags(std::ios::left);

    std::cout << std::setw(idWidth) << "id" << "name" << std::endl;
    std::cout << std::setw(idWidth) << "----" << "----" << std::endl;

    for (auto category : *threadData->categories) {
        std::cout << std::setw(idWidth) << std::setiosflags(std::ios::left) << category.first << category.second <<
        std::endl;
    }

    std::resetiosflags(std::ios::left);
}

void* View::showMainMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

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

    if (!(*threadData->notifications).empty()) {
        std::cout << "-------------" << std::endl;
        std::cout << "notifications" << std::endl;
        std::cout << "-------------" << std::endl;

        for (std::string notification : *threadData->notifications) {
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

    if(((ChooseMenuOptionEvent)event).getOptionChosen()== ChooseMenuOptionEvent::QUIT)
        if(getUserConfirmation()==false)
            return 0;

    threadData->controller->getEventsToServe()->push(std::shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(event)));
}


void* View::showCreateCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

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
        threadData->controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::CREATE_CATEGORY, categoryName, userName, userPassword)));
    }
}

void* View::showDeleteCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    std::string categoryName,userName,userPassword;
    std::cout<<"Deleting catefory:"<<std::endl;

    readCategoryAccessData(categoryName,userName,userPassword,false);

    std::cout << "Delete category "<<categoryName<<"  [y/n]?" << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::DELETE_CATEGORY, categoryName, userName, userPassword)));
    }
}

void* View::showRegisterInCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    std::string categoryName,userName,userPassword;
    std::cout<<"Registering in catefory:"<<std::endl;

    readCategoryAccessData(categoryName,userName,userPassword);

    std::cout << "Category name: " << categoryName << std::endl;
    std::cout << "User name: " << userName << std::endl;
    std::cout << "Register in category? [y/n] " << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::REGISTER_IN_CATEGORY, categoryName, userName,
                                        userPassword)));
    }
}

void* View::showJoinCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    std::string categoryName,userName,userPassword;
    std::cout<<"Joining catefory:"<<std::endl;

    readCategoryAccessData(categoryName,userName,userPassword,false);

    std::cout << "Join category "<<categoryName<<"  [y/n]?" << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::JOIN_CATEGORY, categoryName, userName, userPassword)));
    }
}

void* View::showLeaveCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    std::string categoryName;
    std::cout<<"Leaving category:"<<std::endl;

    std::cout<<"Type category name:";
    std::cin>>categoryName;

    std::cout << "Leave category "<<categoryName<<"  [y/n]?" << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::LEAVE_CATEGORY, categoryName)));
    }
}

void* View::showVisitCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    std::string categoryName;
    std::cout<<"Visiting category:"<<std::endl;

    std::cout<<"Type category name:";
    std::cin>>categoryName;

    std::cout << "Visit category "<<categoryName<<"  [y/n]?" << std::endl;

    char decision;
    std::cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(std::shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::VISIT_CATEGORY, categoryName)));
    }
}






