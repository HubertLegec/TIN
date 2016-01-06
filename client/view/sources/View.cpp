//
// Created by hubert on 15.12.15.
//

#include "../headers/View.h"
#include "../../../clientEvents/headers/ChooseMenuOptionEvent.h"
#include "../../../clientEvents/headers/CategoryAccessEvent.h"
#include "../../../clientEvents/headers/UserAccountEvent.h"
#include "../../../clientEvents/headers/NewMessageEvent.h"
#include "../../../clientEvents/headers/ConfirmMessageEvent.h"
#include "../../../clientEvents/headers/PendingUserEvent.h"
#include <iomanip>

using namespace std;

View::View(Controller *controller) : threadData(controller), viewThread() {
}

View::~View() { pthread_join(viewThread, NULL); }


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
    pthread_create(&viewThread, NULL, showJoinCategorySubMenuThread, &threadData);
}

void View::showLeaveCategorySubMenu(map<long, string> categories) {
    pthread_join(viewThread, NULL);
    *threadData.categories = categories;
    pthread_create(&viewThread, NULL, showLeaveCategorySubMenuThread, &threadData);
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

void View::showReadIncomingMessagesSubMenu(std::vector<std::pair<std::string, std::string>> messages) {
    pthread_join(viewThread, NULL);
    *threadData.messages = messages;
    pthread_create(&viewThread, NULL, showReadIncomingMessagesSubMenuThread, &threadData);
}

void View::showPendingUsersSubMenu(std::vector<PendingUserInfo> pendingUsers) {
    pthread_join(viewThread, NULL);
    *threadData.pendingUsers = pendingUsers;
    pthread_create(&viewThread, NULL, showPendingUsersSubMenuThread, &threadData);
}

void View::showInfo(const std::string &info) {
    pthread_join(viewThread, NULL);
    threadData.info = info;
    pthread_create(&viewThread, NULL, showInfoThread, &threadData);
}

void View::showRemoveUserSubMenu() {
    pthread_join(viewThread, NULL);
    pthread_create(&viewThread, NULL, showRemoveUserSubMenuThread, &threadData);
}

bool View::getUserConfirmation() {
    cout << endl << "Are you sure (y/n)?" << endl;
    char decision;
    cin >> decision;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << endl;
    return decision == 'y' ? true : false;
}


void *View::showCategoryListThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

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

void *View::showMainMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

    cout << "****************************" << endl;
    cout << "Welcome to #RING!" << endl;
    cout << "choose action:" << endl;
    cout << "[a] create user account" << endl;
    cout << "[r] remove user account" << endl;
    cout << "[c] create category" << endl;
    cout << "[d] delete category" << endl;
    cout << "[s] show category list" << endl;
    cout << "[u] sign up category" << endl;
    cout << "[j] join category" << endl;
    cout << "[o] sign out category" << endl;
    cout << "[l] leave category" << endl;
    cout << "[m] send message in category" << endl;
    cout << "[i] open inbox" << endl;
    cout << "[p] see users who want to join your category" << endl;
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
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << endl;

    switch (typed) {
        case 'a':
            event = ChooseMenuOptionEvent::CREATE_USER_ACCOUNT;
            break;
        case 'r':
            event = ChooseMenuOptionEvent::REMOVE_USER_ACCOUNT;
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
        case 'i':
            event = ChooseMenuOptionEvent::OPEN_INBOX;
            break;
        case 'p':
            event = ChooseMenuOptionEvent::PENDING_USERS;
            break;
        case 'q':
            event = ChooseMenuOptionEvent::QUIT;
            break;
    }

    if (event == ChooseMenuOptionEvent::QUIT && !getUserConfirmation()) {
        return 0;
    }

    threadData->controller->getEventsToServe()->push(
            shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(event)));
}


void *View::showCreateCategorySubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

    string categoryName;

    cout << "Creating category:" << endl;
    cout << "Category name: ";
    getline(cin, categoryName);
    cout << endl << "Create category? [y/n] " << endl;

    char decision;
    cin >> decision;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (decision == 'y') {
        threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                new CategoryAccessEvent(CategoryAccessEvent::CREATE_CATEGORY, categoryName)));
    } else {
        threadData->controller->getEventsToServe()->push(
                shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
    }
}

void *View::showDeleteCategorySubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;


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
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (validateChoosenID(id, threadData)) {
        cout << endl << "Delete category " << (*threadData->categories)[id] << "  [y/n]?" << endl;

        char decision;
        cin >> decision;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (decision == 'y') {
            threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                    new CategoryAccessEvent(CategoryAccessEvent::DELETE_CATEGORY, id)));
        } else {
            threadData->controller->getEventsToServe()->push(
                    shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
        }
    } else {
        cout << "ID you typed is invalid!" << endl;
        threadData->controller->getEventsToServe()->push(
                shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
    }
}

void *View::showSignUpCategorySubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

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
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (validateChoosenID(categoryID, threadData)) {
        cout << "Category name: " << (*threadData->categories)[categoryID] << endl;
        cout << "Register in category? [y/n] " << endl;

        char decision;
        cin >> decision;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (decision == 'y') {
            threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                    new CategoryAccessEvent(CategoryAccessEvent::SIGN_UP_CATEGORY, categoryID)));
        } else {
            threadData->controller->getEventsToServe()->push(
                    shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
        }
    } else {
        cout << "ID you typed is invalid!" << endl;
        threadData->controller->getEventsToServe()->push(
                shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
    }
}

void *View::showJoinCategorySubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

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
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (validateChoosenID(categoryID, threadData)) {
        cout << "Join category " << (*threadData->categories)[categoryID] << "  [y/n]?" << endl;

        char decision;
        cin >> decision;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (decision == 'y') {
            threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                    new CategoryAccessEvent(CategoryAccessEvent::JOIN_CATEGORY, categoryID)));
        } else {
            threadData->controller->getEventsToServe()->push(
                    shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
        }
    } else {
        cout << "ID you typed is invalid!" << endl;
        threadData->controller->getEventsToServe()->push(
                shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
    }
}

void *View::showLeaveCategorySubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

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
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (validateChoosenID(categoryID, threadData)) {
        cout << "Leave category " << (*threadData->categories)[categoryID] << "  [y/n]?" << endl;

        char decision;
        cin >> decision;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (decision == 'y') {
            threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                    new CategoryAccessEvent(CategoryAccessEvent::LEAVE_CATEGORY, categoryID)));
        } else {
            threadData->controller->getEventsToServe()->push(
                    shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
        }
    } else {
        cout << "ID you typed is invalid!" << endl;
        threadData->controller->getEventsToServe()->push(
                shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
    }
}

void *View::showSignOutCategorySubMenuThread(void *arg) {

    ThreadData *threadData = (ThreadData *) arg;

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
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (validateChoosenID(categoryID, threadData)) {
        cout << "Sign out category " << (*threadData->categories)[categoryID] << "  [y/n]?" << endl;

        char decision;
        cin >> decision;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (decision == 'y') {
            threadData->controller->getEventsToServe()->push(shared_ptr<CategoryAccessEvent>(
                    new CategoryAccessEvent(CategoryAccessEvent::SIGN_OUT_CATEGORY, categoryID)));
        } else {
            threadData->controller->getEventsToServe()->push(
                    shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
        }
    } else {
        cout << "ID you typed is invalid!" << endl;
        threadData->controller->getEventsToServe()->push(
                shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
    }
}

void *View::showRegisterNewUserSubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

    string userName;
    cout << "You are not registered yet!" << endl;
    cout << "Create your personal account:" << endl;
    cout << "Login: ";
    getline(cin, userName);
    if (getUserConfirmation()) {
        threadData->controller->getEventsToServe()->push(
                shared_ptr<UserAccountEvent>(new UserAccountEvent(UserAccountEvent::CREATE, userName)));
    } else {
        threadData->controller->getEventsToServe()->push(
                shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
    }
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
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (validateChoosenID(categoryID, threadData)) {
        string message;

        cout << endl << "Type message:\n";
        getline(cin, message);

        threadData->controller->getEventsToServe()->push(
                shared_ptr<NewMessageEvent>(new NewMessageEvent(categoryID, message)));
    } else {
        cout << "ID you typed is invalid!" << endl;
        threadData->controller->getEventsToServe()->push(
                shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
    }
}

void *View::showReadIncomingMessagesSubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;

    cout << "Inbox:" << endl;
    cout << "Here you can mark some messages as read." << endl;
    cout << "[y] - yes" << endl << "[n] - no" << endl << "[other] - quit" << endl << endl;
    int position = 0;
    int markedCounter = 0;
    for (auto message : *threadData->messages) {
        cout << "-------------" << endl;
        cout << "Category: " << message.first << endl;
        cout << "---" << endl;
        cout << message.second << endl << endl;
        cout << "\tMark as read [y/n/other]? ";
        char decision;
        cin >> decision;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << endl;

        switch (decision) {
            case 'y': {
                threadData->controller->getEventsToServe()->push(
                        shared_ptr<ConfirmMessageEvent>(new ConfirmMessageEvent(position - markedCounter)));
                position++;
                markedCounter++;
                break;
            }
            case 'n':
                break;
            default:
                threadData->controller->getEventsToServe()->push(
                        shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
                return nullptr;
        }
    }

    cout << "Nothing to show." << endl;
    threadData->controller->getEventsToServe()->push(
            shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
}

void *View::showPendingUsersSubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;
    cout << "Users who want to join to your categories:" << endl;
    cout << "You can agree or reject them." << endl;
    cout << "[y] - yes" << endl << "[n] - no" << endl << "[other] - quit" << endl << endl;

    for (auto pendingUser : *threadData->pendingUsers) {
        cout << "-------------" << endl;
        cout << "Category: " << pendingUser.getCategoryName() << endl;
        cout << "User name: " << pendingUser.getUserName() << endl;
        cout << "\tAccept user [y/n/other]? ";
        char decision;
        cin >> decision;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << endl;

        switch (decision) {
            case 'y': {
                threadData->controller->getEventsToServe()->push(shared_ptr<PendingUserEvent>(
                        new PendingUserEvent(pendingUser.getCategoryID(), pendingUser.getUserID(),
                                             PendingUserEvent::ACCEPT)));
                break;
            }
            case 'n':
                threadData->controller->getEventsToServe()->push(shared_ptr<PendingUserEvent>(
                        new PendingUserEvent(pendingUser.getCategoryID(), pendingUser.getUserID(),
                                             PendingUserEvent::REJECT)));
                break;
            default:
                threadData->controller->getEventsToServe()->push(
                        shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
                return nullptr;
        }
    }

    threadData->controller->getEventsToServe()->push(
            shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
}

void *View::showRemoveUserSubMenuThread(void *arg) {
    ThreadData *threadData = (ThreadData *) arg;
    cout << "This will remove all your messages, you will be removed from all categories you have signed up" << endl;
    cout << "and all your categories will be deleted." << endl;
    if (getUserConfirmation()) {
        threadData->controller->getEventsToServe()->push(
                shared_ptr<UserAccountEvent>(new UserAccountEvent(UserAccountEvent::DELETE, "")));
    } else {
        threadData->controller->getEventsToServe()->push(
                shared_ptr<ChooseMenuOptionEvent>(new ChooseMenuOptionEvent(ChooseMenuOptionEvent::REFRESH)));
    }
}

bool View::validateChoosenID(long id, ThreadData *threadData) {
    if (threadData->categories->find(id) != threadData->categories->end()) {
        return true;
    } else {
        return false;
    }
}