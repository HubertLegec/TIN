#include <iostream>
#include "../logger/easylogging++.h"
#include "../networkMessage/headers/SimpleMessage.h"
#include "controller/headers/Controller.h"
#include "../networkMessage/headers/GetMessage.h"
#include "../networkMessage/headers/CategoryManagementMessage.h"
#include "../networkMessage/headers/UserManagementMessage.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argv, char *argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    LOG(INFO) << "Server started";

    Controller c;
    c.putIncomingMessage(new GetMessage());
    c.putIncomingMessage(new GetMessage());
    c.putIncomingMessage(new GetMessage());

    c.putIncomingMessage(new UserManagementMessage(USER_SIGN_UP,  "11.11", 9999, "radek"));

    c.putIncomingMessage(new CategoryManagementMessage(0, CREATE_CATEGORY, "No elo. Fajna kategoria"));
    c.putIncomingMessage(new CategoryManagementMessage(0, CREATE_CATEGORY, "No elo. Fajna kategoria"));
    c.putIncomingMessage(new CategoryManagementMessage(0, DESTROY_CATEGORY, 0));


    c.run();

    return 0;
}