#include <iostream>
#include "model/headers/User.h"
#include "model/headers/Model.h"
#include "../logger/easylogging++.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argv, char *argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    LOG(INFO) << "Server started";
    shared_ptr<User> radek(new User(0, 1111, "11.11.11.11", "radek"));

    Model model;
    model.addCategory(radek, "tiny");

    // "test" na szybko...
    // TODO testy jednostkowe
    for (auto c : model.getCategories()) {
//        c.second->addMember(radek);
        cout << c.second->getName() << endl;
        cout << c.second->getOwner()->getName() << " " << c.second->getOwner()->getIP() << endl;
    }

    return 0;
}