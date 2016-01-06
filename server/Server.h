#ifndef RING_SEVER_H
#define RING_SEVER_H

#include "controller/headers/Controller.h"

class Server {
private:
    static shared_ptr<Server> serverPtr;
    shared_ptr<Controller> controller;

    Server(string ip, int port);

public:
    static const int DEFAULT_PORT;
    static const string DEFAULT_IP;
    static const string DEFAULT_LOG_PATH;

    static shared_ptr<Server> getServerPtr();

    static shared_ptr<Server> createServer(string ip, int port);

    void cleanUp();

    void start();
};


#endif //RING_SEVER_H
