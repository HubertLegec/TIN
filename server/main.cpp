#include <iostream>
#include "model/headers/User.h"
#include "model/headers/Model.h"

using namespace std;

int main() {
    cout << "Server app!" << endl;
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