#include <iostream>
#include "model/headers/Model.h"
#include "view/headers/View.h"
#include "../logger/easylogging++.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    LOG(INFO) << "Client started";

    string ip;
    int port;
    if (argv == 1) {
        ip = Model::SERVER_DEFAULT_IP;
        port = Model::SERVER_DEFAULT_PORT;
    } else if (argv == 2) {
        ip = argc[1];
        int port = Model::SERVER_DEFAULT_PORT;
    } else if (argv == 3) {
        ip = argc[1];
        port = atoi(argc[2]);
    } else {
        cout << "Wrong arguments!!!\n";
    }

    Model model;
    model.setServerInfo(ip, port);
    Controller controller(&model);
    View view(&controller);
    controller.setView(&view);
    controller.start();
    return 0;
}