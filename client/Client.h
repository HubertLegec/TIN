//
// Created by hubert on 06.01.16.
//

#ifndef RING_CLIENT_H
#define RING_CLIENT_H

#include <memory>
#include <string>
#include "controller/headers/Controller.h"

class Client {
private:
    static std::shared_ptr<Client> clientPtr;
    std::shared_ptr<Controller> controller;
    std::shared_ptr<Model> model;
    std::shared_ptr<View> view;
public:
    Client();

    Client(const std::string &serverIP, int serverPort, const std::string &clientIP, int clientPort);

    static std::shared_ptr<Client> getClientPtr();

    static std::shared_ptr<Client> getClientPtr(const std::string &serverIP, int serverPort,
                                                const std::string &clientIP, int clientPort);

    void start();

    void cleanUp();
};


#endif //RING_CLIENT_H
