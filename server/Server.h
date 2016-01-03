#ifndef RING_SEVER_H
#define RING_SEVER_H

#include "controller/headers/Controller.h"

class Server {
private:
    const static int DEFAULT_PORT;
    const static string DEFAULT_IP;
    static shared_ptr<Server> serverPtr;
    shared_ptr<Controller> controller;

    Server(string ip, int port);

public:
    static shared_ptr<Server> getServerPtr();

    static shared_ptr<Server> getServerPtr(int port);

    static shared_ptr<Server> getServerPtr(string ip, int port);

    void cleanUp();

    void start();
};


#endif //RING_SEVER_H
