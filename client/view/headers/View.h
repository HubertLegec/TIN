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

    };

    View(Controller* controller);
    ~View();

    void showCategoryList(std::map<long, std::string> categories);
    void showMainMenu(std::vector<std::string> notificationsList);

    void showCreateCategorySubMenu();
    void showDeleteCategorySubMenu();
    void showRegisterInCategorySubMenu();
    void showJoinCategorySubMenu();
    void showLeaveCategorySubMenu();
    void showVisitCategorySubMenu();


private:
    static void* showCategoryListThread(void*);
    static void* showMainMenuThread(void*);
    static void* showCreateCategorySubMenuThread(void*);
    static void* showDeleteCategorySubMenuThread(void*);
    static void* showRegisterInCategorySubMenuThread(void*);
    static void* showJoinCategorySubMenuThread(void*);
    static void* showLeaveCategorySubMenuThread(void*);
    static void* showVisitCategorySubMenuThread(void*);

    static void readCategoryAccessData(std::string & categoryName, std::string & userName, std::string & userPassword, bool passwordConfirmation = true);
    static bool getUserConfirmation();

    ThreadData threadData;
    pthread_t viewThread;
};


#endif //RING_VIEW_H
