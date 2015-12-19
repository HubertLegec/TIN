#include <iostream>

#include "model/headers/Model.h"
#include "view/headers/View.h"
#include "../logger/easylogging++.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    LOG(INFO) << "Client started";
    cout << "Client app!" << endl;
    Model model;
    Controller controller(&model);
    View view(&controller);
    controller.setView(&view);
    return 0;
}