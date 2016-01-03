#include <iostream>
#include "../logger/easylogging++.h"
#include "../networkMessage/headers/SimpleMessage.h"
#include "controller/headers/Controller.h"
#include "Server.h"

INITIALIZE_EASYLOGGINGPP

using namespace std;

void handler(int signal) {
    Server::getServerPtr()->cleanUp();
    LOG(FATAL) << "Server stopped by signal " << signal;
}

int main(int argv, char *argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    LOG(INFO) << "Server started";

    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    try {
        Server::getServerPtr()->start();
    } catch (exception &e) {
        LOG(FATAL) << "Server stopped working. Exception log: " << e.what();
    }

    return 0;
}