//
// Created by hubert on 06.01.16.
//

#include "Client.h"

using namespace std;

shared_ptr<Client> Client::clientPtr;

Client::Client() {
    model = make_shared<Model>();
    model->setServerInfo(Model::SERVER_DEFAULT_IP, Model::SERVER_DEFAULT_PORT);
    controller = make_shared<Controller>(model.get());
    view = make_shared<View>(controller.get());
    controller->setView(view.get());
}

Client::Client(const std::string &serverIP, int serverPort, const std::string &clientIP, int clientPort) {
    model = make_shared<Model>();
    model->setServerInfo(serverIP, serverPort);
    model->setMyIP(clientIP);
    model->setMyPort(clientPort);
    controller = make_shared<Controller>(model.get());
    view = make_shared<View>(controller.get());
    controller->setView(view.get());
}

void Client::cleanUp() {
    controller->exit();
}

void Client::start() {
    controller->start();
}

std::shared_ptr<Client> Client::getClientPtr() {
    if (clientPtr) {
        return clientPtr;
    } else {
        clientPtr = make_shared<Client>();
        return clientPtr;
    }
}

std::shared_ptr<Client> Client::getClientPtr(const std::string &serverIP, int serverPort, const std::string &clientIP,
                                             int clientPort) {
    if (clientPtr) {
        return clientPtr;
    } else {
        clientPtr = shared_ptr<Client>(new Client(serverIP, serverPort, clientIP, clientPort));
        return clientPtr;
    }
}
