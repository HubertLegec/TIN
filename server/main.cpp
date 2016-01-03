#include <iostream>
#include "../logger/easylogging++.h"
#include "../networkMessage/headers/SimpleMessage.h"
#include "controller/headers/Controller.h"

INITIALIZE_EASYLOGGINGPP

using namespace std;

Controller *controller = nullptr;

void handler(int signal) {
    if (controller != nullptr)
        delete controller;

    LOG(INFO) << "Server stopped by signal " << signal;
    exit(signal);
}

int main(int argv, char *argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    LOG(INFO) << "Server started";

    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    LOG(FATAL) << "DUPA";

    try {
        controller = new Controller();
        controller->run();
    } catch (exception &e) {
        delete controller;
        LOG(FATAL) << "Server stopped working. Exception log: " << e.what();
    }

    delete controller;
    return 0;
}