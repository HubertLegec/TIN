//
// Created by hubert on 03.12.15.
//

#include <unistd.h>
#include "../headers/NetworkController.h"


void NetworkController::prepareSendThread() {
    pthread_t *thread;
    pthread_create(thread, NULL, startSendThread, this);
}

void *NetworkController::startSendThread(void *param) {
    NetworkController *obj = (NetworkController *) param;
    obj->createSendThread();
}

void NetworkController::createSendThread() {
    while (true) {
        std::pair<SimpleMessage, struct sockaddr *> msg = sendQueue->pop();
        while (sendSockfd = socket(PF_UNIX, SOCK_STREAM, 0) != 0) {
            std::cout << "Coś poszło nie tak podczas otwierania gniazda do wysyłania. Ponawiam próbę...." << std::endl;
        }
        if (!sendMsg(msg.first.toString().data(), msg.second))
            break;
    }
}

bool NetworkController::sendMsg(const char *msg, struct sockaddr *address) {
    if (connect(sendSockfd, address, sizeof(address)) == -1) {
        std::cout << "Coś poszło nie tak, nie udało się utworzyć połączenia to wysłania wiadomości" << std::endl;
        return false;
    }
    //TODO do dorobienia obsługa wysyłania niepełnej wiadomości
    write(sendSockfd, msg, sizeof(msg));
    while (close(sendSockfd) != 0) {
        std::cout << "Coś poszło nie tak podczas zamykania połączenia. Ponawiam próbę...." << std::endl;
    }
    return true;
}

void NetworkController::prepareReceiveThread() {
    while (receiveSockfd = socket(PF_UNIX, SOCK_STREAM, 0) != 0) {
        std::cout << "Coś poszło nie tak podczas otwierania gniazda. Ponawiam próbę...." << std::endl;
    }
    while (bind(receiveSockfd, localAddress, sizeof(localAddress)) != 0) {
        std::cout << "Coś poszło nie tak podczas przypisywania adresu. Ponawiam próbę...." << std::endl;
    }
    //TODO drugi parametr oznacza liczbę połącznień oczekujących na accept - do przemyślenia
    while (listen(receiveSockfd, 10) != 0) {
        std::cout << "Coś poszło nie tak podczas uruchamiania oczekiwania na połaczenia. Ponawiam próbę...." <<
        std::endl;
    }
    pthread_t *thread;
    pthread_create(thread, NULL, startReceiveThread, this);
}

void *NetworkController::startReceiveThread(void *param) {
    NetworkController *obj = (NetworkController *) param;
    obj->createReceiveThread();

}

void NetworkController::createReceiveThread() {
    //TODO do obsłużenia mechanizm zamykania wątków
    while (true) {
        sockaddr *senderAddress = new sockaddr();
        socklen_t *senderAddresLen;
        int senderSockfd = accept(receiveSockfd, senderAddress, senderAddresLen);
        if (senderSockfd == -1) {
            //TODO błąd do obsłużenia
            std::cout << "Wystąpił błąd podczas odbierania połączenia" << std::endl;
            break;
        }
        std::cout << "Poprawnie odebrałem wiadomość. Rozpoczynam procesowanie" << std::endl;

    }
}

void NetworkController::receiveMsg(struct sockaddr *senderAddress, int senderSockfd) {
    char *buffer = new char[1000];
    int msgLength;
    while (msgLength >= 128) {
        msgLength = read(senderSockfd, buffer, 128);
    }
    close(senderSockfd);
    //TODO dodać do kolejki
    SimpleMessage msg;
    receiveQueue->push(msg);
}

