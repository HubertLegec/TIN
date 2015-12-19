//
// Created by hubert on 15.12.15.
//

#ifndef RING_VIEW_H
#define RING_VIEW_H

#include <string>
#include <map>
#include "../../controller/headers/Controller.h"
class Controller;

class View {
private:
    Controller* controller;

public:
    View(Controller* controller);
    void showCategoryList(std::map<long, std::string> categories);
};


#endif //RING_VIEW_H
