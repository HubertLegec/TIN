#include "Server.h"

const int Server::DEFAULT_PORT = 8888;
const string Server::DEFAULT_IP = "127.0.1.1";
shared_ptr<Server> Server::serverPtr;

Server::Server(string ip, int port) {
    controller.reset(new Controller(ip, port));
}

shared_ptr<Server> Server::getServerPtr() {
    if (serverPtr)
        return serverPtr;
    else
        return shared_ptr<Server>(new Server(DEFAULT_IP, DEFAULT_PORT));
}

shared_ptr<Server> Server::getServerPtr(int port) {
    if (serverPtr)
        return serverPtr;
    else
        return getServerPtr(DEFAULT_IP, port);
}

shared_ptr<Server> Server::getServerPtr(string ip, int port) {
    if (serverPtr)
        return serverPtr;
    else
        return shared_ptr<Server>(new Server(ip, port));
}

void Server::start() {
    controller->run();
}

void Server::cleanUp() {
    controller->cleanUp();
}
