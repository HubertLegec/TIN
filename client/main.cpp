#include <iostream>

#include "model/headers/Model.h"
#include "controller/headers/Controller.h"

using namespace std;

int main() {
    cout << "Client app!" << endl;
    Model model;
    Controller controller(model);
    View view(controller);
    controller.setView(view);
    return 0;
}