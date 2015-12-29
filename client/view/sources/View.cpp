//
// Created by hubert on 15.12.15.
//

#include "../headers/View.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../../clientEvents/headers/CategoryAccessEvent.h"
#include "../../../clientEvents/headers/UserAccountEvent.h"
#include <iomanip>

using namespace std;

View::View(Controller * controller) : threadData(controller),viewThread() {
}

View::~View() { pthread_join(viewThread,NULL); }


void View::showCategoryList(map<long, string> categories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = categories;
    pthread_create(&viewThread, NULL, showCreateCategorySubMenuThread, &threadData);
}


void View::showMainMenu(vector<string> notificationsList) {

    pthread_join(viewThread, NULL);
    *threadData.notifications = notificationsList;
    pthread_create(&viewThread, NULL, showMainMenuThread, &threadData);

}

void View::showCreateCategorySubMenu() {
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showCreateCategorySubMenuThread, &threadData);
}

void View::showDeleteCategorySubMenu(std::map<long, std::string> myCategories)
{
    pthread_join(viewThread, NULL);
    *threadData.categories = myCategories;
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

void View::showRegisterNewUserSubMenu() {
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showRegisterNewUserSubMenuThread, &threadData);
}

void View::readCategoryAccessData(string &categoryName, string &userName, string &userPassword, bool confirmPassword)
{
    string passwordConfirmation;

    cout << "Type category name:";
    cin >> categoryName;

    cout << "Type user name:";
    cin >> userName;

    while(true) {
        cout << "Type your password:";
        cin >> userPassword;

        if(!confirmPassword)
            break;

        cout << "Confirm Password:";
        cin >> passwordConfirmation;

        if (userPassword == passwordConfirmation)
            break;
        else
            cout << "Passwords don't match!" << endl;
    }
}

bool View::getUserConfirmation() {
    cout << endl << "Are you sure (y/n)?" << endl;
    char decision;
    cin >> decision;
    cout << endl;
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
    cout << "Category list:" << endl;

    cout << setiosflags(ios::left);

    cout << setw(idWidth) << "id" << "name" << endl;
    cout << setw(idWidth) << "----" << "----" << endl;

    for (auto category : *threadData->categories) {
        cout << setw(idWidth) << setiosflags(ios::left) << category.first << category.second <<
        endl;
    }

    resetiosflags(ios::left);
}

void* View::showMainMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    cout << "Welcome to #RING!" << endl;
    cout << "choose action:" << endl;
    cout << "[c] create category" << endl;
    cout << "[d] delete category" << endl;
    cout << "[s] show category list" << endl;
    cout << "[r] register in category" << endl;
    cout << "[j] join category" << endl;
    cout << "[v] visit category" << endl;
    cout << "[l] leave category" << endl;
    cout << "[q] quit" << endl;

    if (!(*threadData->notifications).empty()) {
        cout << "-------------" << endl;
        cout << "notifications" << endl;
        cout << "-------------" << endl;

        for (string notification : *threadData->notifications) {
            cout << notification << endl;
        }
        cout << "-------------" << endl;

    }
    cout << "[other] refresh" << endl;

    char typed;

    ChooseMenuOptionEvent::OptionChosen event = ChooseMenuOptionEvent::REFRESH;


    cout << ">";
    cin.clear();
    cin.sync();
    typed = cin.get();

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

    threadData->controller->getEventsToServe()->push(
            shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(event)));
}


void* View::showCreateCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    string categoryName;


    cout << "Creating category:" << endl;
    cout << "Category name: ";
    cin >> categoryName;
    cout << endl << "Create category? [y/n] " << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::CREATE_CATEGORY, categoryName)));
    }
}

void* View::showDeleteCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    int idWidth = 21; //Max long type decimal digits number + 1.
    cout << "Deleting catefory:" << endl;
    cout << "Your categories:" << endl;

    cout << setiosflags(ios::left);

    cout << setw(idWidth) << "id" << "name" << endl;
    cout << setw(idWidth) << "----" << "----" << endl;

    for (auto category : *threadData->categories) {
        cout << setw(idWidth) << setiosflags(ios::left) << category.first << category.second <<
        endl;
    }

    resetiosflags(ios::left);

    long id;
    cout << "Category to delete ID: ";
    cin >> id;

    cout << endl << "Delete category " << (*threadData->categories)[id] << "  [y/n]?" << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::DELETE_CATEGORY, id)));
    }
}

void* View::showRegisterInCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    string categoryName, userName, userPassword;
    cout << "Registering in catefory:" << endl;

    readCategoryAccessData(categoryName,userName,userPassword);

    cout << "Category name: " << categoryName << endl;
    cout << "Register in category? [y/n] " << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::REGISTER_IN_CATEGORY, categoryName)));
    }
}

void* View::showJoinCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    string categoryName, userName, userPassword;
    cout << "Joining catefory:" << endl;

    readCategoryAccessData(categoryName,userName,userPassword,false);

    cout << "Join category " << categoryName << "  [y/n]?" << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::JOIN_CATEGORY, categoryName)));
    }
}

void* View::showLeaveCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    string categoryName;
    cout << "Leaving category:" << endl;

    cout << "Type category name:";
    cin >> categoryName;

    cout << "Leave category " << categoryName << "  [y/n]?" << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::LEAVE_CATEGORY, categoryName)));
    }
}

void *View::showVisitCategorySubMenuThread(void *arg) {

    ThreadData * threadData = (ThreadData*)arg;

    string categoryName;
    cout << "Visiting category:" << endl;

    cout << "Type category name:";
    cin >> categoryName;

    cout << "Visit category " << categoryName << "  [y/n]?" << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::VISIT_CATEGORY, categoryName)));
    }
}

void *View::showRegisterNewUserSubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

    string userName;
    cout << "You are not registered yet!" << endl;
    cout << "Create your personal account:" << endl;
    cout << "Login: ";
    cin >> userName;

    threadData->controller->getEventsToServe()->push(shared_ptr<UserAccountEvent>(new UserAccountEvent(userName)));
}






