#include <iostream>

#include "model/Model.h"

using namespace std;

int main() {
    Model m;
    cout << "Client app!" << endl;
    cout << m.getX() << endl;
    return 0;
}