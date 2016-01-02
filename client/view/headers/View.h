//
// Created by hubert on 15.12.15.
//

#ifndef RING_VIEW_H
#define RING_VIEW_H

#include <string>
#include <map>
#include <vector>
#include "../../controller/headers/Controller.h"
class Controller;

class View {
public:

    struct ThreadData
    {
        ThreadData(Controller * controller_):
                 controller(controller_),
                 notifications(new std::vector<std::string>()),
                 categories(new std::map<long, std::string>()) {}

        Controller * controller;

        std::shared_ptr<std::vector<std::string>> notifications;
        std::shared_ptr<std::map<long, std::string>> categories;
        std::string info;
    };

    View(Controller* controller);
    ~View();

    void showCategoryList(std::map<long, std::string> categories);
    void showMainMenu(std::vector<std::string> notificationsList);

    void showCreateCategorySubMenu();

    void showDeleteCategorySubMenu(std::map<long, std::string> myCategories);

    void showSignUpCategorySubMenu(std::map<long, std::string> myCategories);

    void showJoinCategorySubMenu(std::map<long, std::string> myCategories);

    void showLeaveCategorySubMenu(std::map<long, std::string> myCategories);

    void showSignOutCategorySubMenu(std::map<long, std::string> myCategories);
    void showRegisterNewUserSubMenu();

    void showInfo(const std::string &info);

    void sendMessageInCategorySubMenu(std::map<long, std::string> myCategories);

private:
    static const int ID_WIDTH = 21; //Max long type decimal digits number + 1.

    static void* showCategoryListThread(void*);
    static void* showMainMenuThread(void*);
    static void* showCreateCategorySubMenuThread(void*);
    static void* showDeleteCategorySubMenuThread(void*);

    static void *showSignUpCategorySubMenuThread(void *);
    static void* showJoinCategorySubMenuThread(void*);
    static void* showLeaveCategorySubMenuThread(void*);

    static void *showSignOutCategorySubMenuThread(void *);

    static void *showRegisterNewUserSubMenuThread(void *);

    static void *showInfoThread(void *);

    static void *sendMessageInCategorySubMenuThread(void *);

    static void readCategoryAccessData(std::string & categoryName, std::string & userName, std::string & userPassword, bool passwordConfirmation = true);
    static bool getUserConfirmation();

    ThreadData threadData;
    pthread_t viewThread;
};


#endif //RING_VIEW_H
