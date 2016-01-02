#include <iostream>
#include "model/headers/Model.h"
#include "view/headers/View.h"
#include "../logger/easylogging++.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[]) {
    START_EASYLOGGINGPP(argv, argc);
    LOG(INFO) << "Client started";

    if (argv == 1 && strcmp("-h", argc[1]) == 0) {
        cout << "******* RING help *******\n";
        cout << "'-h' - help\n";
        cout << "'-ip [param]' - set client ip\n";
        cout << "'-port [param]' - set client port\n";
        cout << "'-sip [param]' - set server ip\n";
        cout << "'-sport [param]' - set server port\n";
        cout << "Some or all of these parameters can be omitted. Default parameters will be set then.\n";
        return 0;
    } else if (argv % 2 != 0) {
        string serverIP = Model::SERVER_DEFAULT_IP;
        int serverPort = Model::SERVER_DEFAULT_PORT;
        string myIP = Model::CLIENT_DEFAULT_IP;
        int myPort = Model::CLIENT_DEFAULT_PORT;

        int i = 1;
        while (i < argv) {
            if (strcmp(argc[i], "-ip") == 0) {
                serverIP = string(argc[i + 1]);
            } else if (strcmp(argc[i], "-port") == 0) {
                serverPort = atoi(argc[i + 1]);
            } else if (strcmp(argc[i], "-sip") == 0) {
                myIP = string(argc[i + 1]);
            } else if (strcmp(argc[i], "-sport") == 0) {
                myPort = atoi(argc[i + 1]);
            } else {
                cout << "Wrong arguments!!!/n See acceptable possibilities using '-h' flag\n";
                return -1;
            }
            i += 2;
        }

        Model model;
        model.setServerInfo(serverIP, serverPort);
        model.setMyIP(myIP);
        model.setMyPort(myPort);
        Controller controller(&model);
        View view(&controller);
        controller.setView(&view);
        controller.start();
        return 0;
    } else {
        cout << "Wrong arguments!!!/n See acceptable possibilities using '-h' flag\n";
        return -1;
    }
}