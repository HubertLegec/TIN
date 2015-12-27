#include <iostream>
#include "../logger/easylogging++.h"
#include "../networkMessage/headers/SimpleMessage.h"
#include "controller/headers/Controller.h"
#include "../networkMessage/headers/GetMessage.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argv, char *argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    LOG(INFO) << "Server started";

    Model model;
    auto radek = model.createNewUser("radek", 1111, "11.11.11.11");
    auto radek1 = model.createNewUser("Radek!", 1111, "121212");
    model.addCategory(radek, "tiny");
    model.addCategory(radek1, "tiny2");
    model.addMemberToCategory(radek1, 0);

    cout << model.getCategory(0L)->getMembers()->getUser()->getName() << endl;
    for (auto i = model.getCategory(0L)->getMembers()->getRightNeighbour();
         i != model.getCategory(0L)->getMembers(); i = i->getRightNeighbour()) {

        cout << i->getUser()->getName() << endl;
    }

    Controller c;
    c.putIncomingMessage(new GetMessage());
    c.putIncomingMessage(new GetMessage());
    c.putIncomingMessage(new GetMessage());


    c.run();

    // "test" na szybko...
    // TODO testy jednostkowe
//    for (auto c : model.getCategories()) {
//        cout << c.second->getName() << endl;
//        cout << c.second->getOwner()->getName() << " " << c.second->getOwner()->getIP() << endl;
//    }

    return 0;
}