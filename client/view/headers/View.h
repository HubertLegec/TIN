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
private:
    Controller* controller;

public:
    View(Controller* controller);
    void showCategoryList(std::map<long, std::string> categories);
    void showMainMenu();
    void showMainMenuAndNotifications(std::vector<std::string> notificationsList);
};


#endif //RING_VIEW_H
