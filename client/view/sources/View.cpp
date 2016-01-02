//
// Created by hubert on 15.12.15.
//

#include "../headers/View.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../../clientEvents/headers/CategoryAccessEvent.h"
#include "../../../clientEvents/headers/UserAccountEvent.h"
#include "../../../clientEvents/headers/NewMessageEvent.h"
#include <iomanip>

using namespace std;

View::View(Controller * controller) : threadData(controller),viewThread() {
}

View::~View() { pthread_join(viewThread,NULL); }


void View::showCategoryList(map<long, string> categories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = categories;
    pthread_create(&viewThread, NULL, showCategoryListThread, &threadData);
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

void View::showDeleteCategorySubMenu(std::map<long, std::string> myCategories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = myCategories;
    pthread_create(&viewThread, NULL, showDeleteCategorySubMenuThread, &threadData);
}

void View::showSignUpCategorySubMenu(map<long, string> categories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = categories;
    pthread_create(&viewThread, NULL, showSignUpCategorySubMenuThread, &threadData);
}

void View::showJoinCategorySubMenu(map<long, string> categories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = categories;
    pthread_create(&viewThread, NULL, showJoinCategorySubMenuThread,&threadData);
}

void View::showLeaveCategorySubMenu(map<long, string> categories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = categories;
    pthread_create(&viewThread, NULL, showLeaveCategorySubMenuThread,&threadData);
}

void View::showSignOutCategorySubMenu(map<long, string> categories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = categories;
    pthread_create(&viewThread, NULL, showSignOutCategorySubMenuThread, &threadData);
}

void View::showRegisterNewUserSubMenu() {
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showRegisterNewUserSubMenuThread, &threadData);
}

void View::sendMessageInCategorySubMenu(std::map<long, std::string> myCategories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = myCategories;
    pthread_create(&viewThread, NULL, sendMessageInCategorySubMenuThread, &threadData);
}

void View::showInfo(const std::string &info) {
    pthread_join(viewThread, NULL);
    threadData.info = info;
    pthread_create(&viewThread, NULL, showInfoThread, &threadData);
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
    cout << "[a] create user account" << endl;
    cout << "[c] create category" << endl;
    cout << "[d] delete category" << endl;
    cout << "[s] show category list" << endl;
    cout << "[u] sign up category" << endl;
    cout << "[j] join category" << endl;
    cout << "[o] sign out category" << endl;
    cout << "[l] leave category" << endl;
    cout << "[m] send message in category" << endl;
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
    cin >> typed;

    switch (typed) {
        case 'a':
            event = ChooseMenuOptionEvent::CREATE_USER_ACCOUNT;
            break;
        case 'c':
            event = ChooseMenuOptionEvent::CREATE_CATEGORY;
            break;
        case 'd':
            event = ChooseMenuOptionEvent::DELETE_CATEGORY;
            break;
        case 's':
            event = ChooseMenuOptionEvent::SHOW_CATEGORY_LIST;
            break;
        case 'u':
            event = ChooseMenuOptionEvent::SIGN_UP_CATEGORY;
            break;
        case 'j':
            event = ChooseMenuOptionEvent::JOIN_CATEGORY;
            break;
        case 'o':
            event = ChooseMenuOptionEvent::SIGN_OUT_CATEGORY;
            break;
        case 'l':
            event = ChooseMenuOptionEvent::LEAVE_CATEGORY;
            break;
        case 'm':
            event = ChooseMenuOptionEvent::SEND_MESSAGE;
            break;
        case 'q':
            event = ChooseMenuOptionEvent::QUIT;
            break;
    }

    if(((ChooseMenuOptionEvent)event).getOptionChosen()== ChooseMenuOptionEvent::QUIT) if (getUserConfirmation() ==
                                                                                           false) {
            return 0;
    }

    threadData->controller->getEventsToServe()->push(
            shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(event)));
}


void* View::showCreateCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    string categoryName;

    cout << "Creating category:" << endl;
    cout << "Category name: ";
    cin.ignore();
    getline(cin, categoryName);
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


    cout << "Deleting category:" << endl;
    cout << "Your categories:" << endl;

    cout << setiosflags(ios::left);

    cout << setw(ID_WIDTH) << "id" << "name" << endl;
    cout << setw(ID_WIDTH) << "----" << "----" << endl;

    for (auto category : *threadData->categories) {
        cout << setw(ID_WIDTH) << setiosflags(ios::left) << category.first << category.second <<
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

void *View::showSignUpCategorySubMenuThread(void *arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    long categoryID;
    cout << "Singing up category:" << endl;

    cout << setiosflags(ios::left);

    cout << setw(ID_WIDTH) << "id" << "name" << endl;
    cout << setw(ID_WIDTH) << "----" << "----" << endl;

    for (auto category : *threadData->categories) {
        cout << setw(ID_WIDTH) << setiosflags(ios::left) << category.first << category.second <<
        endl;
    }

    resetiosflags(ios::left);

    cout << "Type category id:";
    cin >> categoryID;

    cout << "Category name: " << (*threadData->categories)[categoryID] << endl;
    cout << "Register in category? [y/n] " << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::SIGN_UP_CATEGORY, categoryID)));
    }
}

void* View::showJoinCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    long categoryID;
    cout << "Joining category:" << endl;

    cout << "Your categories: " << endl;

    cout << setiosflags(ios::left);

    cout << setw(ID_WIDTH) << "id" << "name" << endl;
    cout << setw(ID_WIDTH) << "----" << "----" << endl;

    for (auto category : *threadData->categories) {
        cout << setw(ID_WIDTH) << setiosflags(ios::left) << category.first << category.second <<
        endl;
    }

    resetiosflags(ios::left);

    cout << "Type category id:";
    cin >> categoryID;

    cout << "Join category " << (*threadData->categories)[categoryID] << "  [y/n]?" << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::JOIN_CATEGORY, categoryID)));
    }
}

void* View::showLeaveCategorySubMenuThread(void * arg)
{
    ThreadData * threadData = (ThreadData*)arg;

    long categoryID;
    cout << "Leaving category:" << endl;

    cout << "Your categories: " << endl;

    cout << setiosflags(ios::left);

    cout << setw(ID_WIDTH) << "id" << "name" << endl;
    cout << setw(ID_WIDTH) << "----" << "----" << endl;

    for (auto category : *threadData->categories) {
        cout << setw(ID_WIDTH) << setiosflags(ios::left) << category.first << category.second <<
        endl;
    }

    resetiosflags(ios::left);

    cout << "Type category id:";
    cin >> categoryID;

    cout << "Leave category " << (*threadData->categories)[categoryID] << "  [y/n]?" << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::LEAVE_CATEGORY, categoryID)));
    }
}

void *View::showSignOutCategorySubMenuThread(void *arg) {

    ThreadData * threadData = (ThreadData*)arg;

    long categoryID;
    cout << "Sign out category:" << endl;
    cout << "Your categories: " << endl;

    cout << setiosflags(ios::left);

    cout << setw(ID_WIDTH) << "id" << "name" << endl;
    cout << setw(ID_WIDTH) << "----" << "----" << endl;

    for (auto category : *threadData->categories) {
        cout << setw(ID_WIDTH) << setiosflags(ios::left) << category.first << category.second <<
        endl;
    }

    resetiosflags(ios::left);

    cout << "Type category id:";
    cin >> categoryID;

    cout << "Sign out category " << (*threadData->categories)[categoryID] << "  [y/n]?" << endl;

    char decision;
    cin >> decision;
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::SIGN_OUT_CATEGORY, categoryID)));
    }
}

void *View::showRegisterNewUserSubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

    string userName;
    cout << "You are not registered yet!" << endl;
    cout << "Create your personal account:" << endl;
    cout << "Login: ";
    cin >> userName;

    threadData->controller->getEventsToServe()->push(
            shared_ptr<UserAccountEvent>(new UserAccountEvent(UserAccountEvent::CREATE, userName)));
}

void *View::showInfoThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;
    cout << "*****" << endl;
    cout << threadData->info << endl;
    cout << "*****" << endl;
}

void *View::sendMessageInCategorySubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

    long categoryID;
    cout << "Send message in one of your categories:" << endl;
    cout << "Your categories: " << endl;

    cout << setiosflags(ios::left);

    cout << setw(ID_WIDTH) << "id" << "name" << endl;
    cout << setw(ID_WIDTH) << "----" << "----" << endl;

    for (auto category : *threadData->categories) {
        cout << setw(ID_WIDTH) << setiosflags(ios::left) << category.first << category.second <<
        endl;
    }

    resetiosflags(ios::left);

    cout << "Type category id:";
    cin >> categoryID;

    string message;

    cout << endl << "Type message:\n";
    cin.ignore();
    getline(cin, message);

    threadData->controller->getEventsToServe()->push(
            shared_ptr<NewMessageEvent>(new NewMessageEvent(categoryID, message)));
}


