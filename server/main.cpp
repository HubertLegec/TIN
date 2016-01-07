#include <iostream>
#include "../logger/easylogging++.h"
#include "../networkMessage/headers/SimpleMessage.h"
#include "controller/headers/Controller.h"
#include "Server.h"
#include "utils/ServerGlobalConstants.h"

INITIALIZE_EASYLOGGINGPP

using namespace std;

void handler(int signal) {
    LOG(INFO) << "Server program interrupted by signal " << signal;
    Server::getServerPtr()->cleanUp();
}

void showHelp() {
    cout << "******* RING help *******" << endl;
    cout << "Server program arguments:" << endl;
    cout << "-ip [param]  -  set server ip" << endl;
    cout << "-port [param]  -  set server port" << endl;
    cout << "'-logPath [param]' - absolute path to log file" << endl;
}

void badInputArguments() {
    cout << "Bad input arguments!" << endl;
    showHelp();
    exit(ServerGlobalConstant::BAD_INPUT_ARG);
}

int main(int argc, char *argv[]) {
    string ip = Server::DEFAULT_IP;
    int port = Server::DEFAULT_PORT;
    string logFilePath = Server::DEFAULT_LOG_PATH;

    if (argc > 1) {
        if (argc == 2) {
            if (strcmp("-h", argv[1]) != 0 && strcmp("-help", argv[1]) != 0)
                badInputArguments();

            showHelp();
            return 0;
        } else if (argc % 2 != 0) {
            for (int i = 1; i < argc; i += 2) {
                if (strcmp("-ip", argv[i]) == 0)
                    ip = argv[i + 1];
                else if (strcmp("-port", argv[i]) == 0)
                    port = stoi(argv[i + 1]);
                else if (strcmp("-logPath", argv[i]) == 0)
                    logFilePath = argv[i + 1];
                else {
                    badInputArguments();
                }
            }
        } else {
            badInputArguments();
        }
    }

    START_EASYLOGGINGPP(argc, argv);
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Filename, logFilePath.c_str());
    defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
    defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
    defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "20000000");
    el::Loggers::reconfigureLogger("default", defaultConf);
    LOG(INFO) << "Server started with parameters: " << endl
    << "Server IP: " << ip << endl
    << "Server port: " << port << endl;

    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    try {
        Server::createServer(ip, port)->start();
    } catch (exception &e) {
        Server::getServerPtr()->cleanUp();
        LOG(ERROR) << "Server stopped working. Exception log: " << e.what();
    }

    return 0;
}