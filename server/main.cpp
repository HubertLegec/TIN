#include <iostream>
#include "../logger/easylogging++.h"
#include "../networkMessage/headers/SimpleMessage.h"
#include "controller/headers/Controller.h"
#include "../networkMessage/headers/CategoryManagementMessage.h"
#include "../networkMessage/headers/UserManagementMessage.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argv, char *argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    LOG(INFO) << "Server started";

    try {
        Controller c;

        c.run();
    } catch (exception &e) {
        LOG(ERROR) << "Server stopped working. Exception log: " << e.what();
    }

    return 0;
}