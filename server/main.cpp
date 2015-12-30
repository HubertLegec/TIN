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
        c.putIncomingMessage(new UserManagementMessage(CREATE_USER_ACCOUNT, "11.11", 9999, "radek"));
        c.putIncomingMessage(new UserManagementMessage(CREATE_USER_ACCOUNT, "11.11", 9999, "radek2"));

        c.putIncomingMessage(new CategoryManagementMessage(1, CREATE_CATEGORY, "No elo. Fajna kategoria"));
        c.putIncomingMessage(new CategoryManagementMessage(2, CREATE_CATEGORY, "Kategoria 2"));
        c.putIncomingMessage(new CategoryManagementMessage(1, DESTROY_CATEGORY, 0));

        c.run();
    } catch (exception &e) {
        LOG(ERROR) << "Server stopped working. Exception log: " << e.what();
    }

    return 0;
}