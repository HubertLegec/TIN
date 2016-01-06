#include <iostream>
#include "model/headers/Model.h"
#include "view/headers/View.h"
#include "../logger/easylogging++.h"
#include "Client.h"

using namespace std;

void handler(int signal) {
    Client::getClientPtr()->cleanUp();
    usleep(500000);
    LOG(FATAL) << "Client stopped by signal " << signal;
}

bool isValidIPAddress(const string &address) {
    struct sockaddr_in sa;
    bool isIPv4 = inet_pton(AF_INET, address.c_str(), &(sa.sin_addr)) != 0;
    struct sockaddr_in6 sav6;
    bool isIPv6 = inet_pton(AF_INET6, address.c_str(), &(sav6.sin6_addr)) != 0;
    return isIPv4 || isIPv6;
}


INITIALIZE_EASYLOGGINGPP


int main(int argv, char* argc[]) {

    if (argv == 2 && strcmp("-h", argc[1]) == 0) {
        cout << "******* RING help *******\n";
        cout << "'-h' - help\n";
        cout << "'-ip [param]' - set client ip\n";
        cout << "'-port [param]' - set client port\n";
        cout << "'-sip [param]' - set server ip\n";
        cout << "'-sport [param]' - set server port\n";
        cout << "'-logFile [param]' - absolute path to log file\n";
        cout << "Some or all of these parameters can be omitted. Default parameters will be set then.\n";
        return 0;
    } else if (argv % 2 != 0) {
        signal(SIGINT, handler);
        signal(SIGTERM, handler);

        string serverIP = Model::SERVER_DEFAULT_IP;
        int serverPort = Model::SERVER_DEFAULT_PORT;
        string myIP = Model::CLIENT_DEFAULT_IP;
        int myPort = Model::CLIENT_DEFAULT_PORT;
        string logFile = "/tmp/logs/ringClient.log";

        int i = 1;
        while (i < argv) {
            if (strcmp(argc[i], "-ip") == 0) {
                myIP = string(argc[i + 1]);
                if (!isValidIPAddress(myIP)) {
                    cout << "Client IP address is not valid IP address!\n";
                    return -1;
                }
            } else if (strcmp(argc[i], "-port") == 0) {
                myPort = atoi(argc[i + 1]);
            } else if (strcmp(argc[i], "-sip") == 0) {
                serverIP = string(argc[i + 1]);
                if (!isValidIPAddress(serverIP)) {
                    cout << "Server IP address is not valid IP address!\n";
                    return -1;
                }
            } else if (strcmp(argc[i], "-sport") == 0) {
                serverPort = atoi(argc[i + 1]);
            } else if (strcmp(argc[i], "-logFile") == 0) {
                logFile = string(argc[i + 1]);
            } else {
                cout << "Wrong arguments!!!/n See acceptable possibilities using '-h' flag\n";
                return -1;
            }
            i += 2;
        }

        START_EASYLOGGINGPP(argv, argc);
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.setGlobally(el::ConfigurationType::Filename, logFile.c_str());
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "20000000"); //something about 20MB
        el::Loggers::reconfigureLogger("default", defaultConf);
        LOG(INFO) << "Client started with parameters:\nServer IP: " << serverIP << "\nServer port: " << serverPort <<
        "\nClient port: " << myPort;

        Client::getClientPtr(serverIP, serverPort, myIP, myPort)->start();
        return 0;
    } else {
        cout << "Wrong arguments!!!/n See acceptable possibilities using '-h' flag\n";
        return -1;
    }
}