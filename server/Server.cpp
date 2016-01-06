#include "Server.h"

shared_ptr<Server> Server::serverPtr;
const int Server::DEFAULT_PORT = 8888;
const string Server::DEFAULT_IP = "127.0.1.1";
const string Server::DEFAULT_LOG_PATH = "/tmp/logs/serverLog.log";

Server::Server(string ip, int port) {
    controller.reset(new Controller(ip, port));
}

shared_ptr<Server> Server::getServerPtr() {
    if (serverPtr)
        return serverPtr;
    else
        return shared_ptr<Server>();
}

shared_ptr<Server> Server::createServer(string ip, int port) {
    if (serverPtr)
        return serverPtr;
    else
        return (serverPtr = shared_ptr<Server>(new Server(ip, port)));
}

void Server::start() {
    controller->run();
}

void Server::cleanUp() {
    controller->cleanUp();
}
