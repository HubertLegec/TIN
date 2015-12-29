//
// Created by hubert on 03.12.15.
//

#include <unistd.h>
#include "../headers/NetworkController.h"
#include "../../logger/easylogging++.h"


void NetworkController::prepareSendThread() {
    createThread(sendSystemThread, startSendThread);
}

void NetworkController::createThread(pthread_t *thread, void *(*function)(void *)) {
    int rc;
    pthread_create(thread, NULL, function, this);
    rc = pthread_join(*thread, NULL);
    if (rc) {
        LOG(ERROR) << "Unable to join thread" << rc;
        exit(-1);
    }
}

void *NetworkController::startSendThread(void *param) {
    NetworkController *obj = (NetworkController *) param;
    obj->createSendThread();
}

void NetworkController::createSendThread() {
    while (true) {
        std::shared_ptr<MessageWrapper> msg = sendQueue->pop();
        struct addrinfo *serverInfo = prepareConncetionWithReceiver(msg);
        if (!sendMsg(msg->getMessage())) {
            LOG(ERROR) << "Couldnt send msg";
            break;
        }
        freeaddrinfo(serverInfo);
    }
    pthread_exit(NULL);
}

struct addrinfo *NetworkController::prepareConncetionWithReceiver(std::shared_ptr<MessageWrapper> msg) {
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    const char *hostname = msg->getIP().c_str();
    std::string s = std::to_string(msg->getPort());
    char const *port = s.c_str();
    if ((getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        LOG(ERROR) << "Didnt find address with hostname: " << hostname << " port: " << port;
        exit(1);
    }
    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sendSockfd = socket(p->ai_family, p->ai_socktype,
                                 p->ai_protocol)) == -1) {
            LOG(INFO) << "Not created socket. Retrying...";
            continue;
        }

        if (connect(sendSockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sendSockfd);
            LOG(INFO) << "Not connected. Retrying...";
            continue;
        }

        return servinfo; // if we get here, we must have connected successfully
    }
    LOG(ERROR) << "Failed to connect";
}

bool NetworkController::sendMsg(std::shared_ptr<SimpleMessage> msg) {
    //TODO do dorobienia obsługa wysyłania niepełnej wiadomości
    const char *serializedMsg = serializeMsg(msg);
    write(sendSockfd, serializedMsg, sizeof(msg));
    int trialCounter = 1;
    while (trialCounter < 4 || close(sendSockfd) != 0) {
        LOG(INFO) << "Something went wrong during closing connection. Retrying..." << trialCounter++;
    }
    if (trialCounter > 3) {
        LOG(ERROR) << "Something went wrong during closing connection.";
        return false;
    }
    return true;
}

const char *NetworkController::serializeMsg(std::shared_ptr<SimpleMessage> msg) {
    std::stringstream ss;
    cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
    oarchive(*msg); // Write the data to the archive
    const std::string tmp = ss.str();
    return tmp.c_str();

}

void NetworkController::prepareReceiveThread() {

    if (prepareListeningSocket() == NULL)
        exit(1);
    //TODO drugi parametr - liczba połączeń oczekujących, do ogarnięcia
    listen(receiveSockfd, 10);
    createThread(receiveSystemThread, startReceiveThread);
}

struct addrinfo *NetworkController::prepareListeningSocket() {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP address

    if ((getaddrinfo(NULL, "80", &hints, &servinfo)) != 0) {
        LOG(ERROR) << "Something went wrong during closing connection.";
        //TODO obsługa
//        exit(1);
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            LOG(INFO) << "Not created socket. Retrying...";
            continue;
        }

        if (receiveSockfd = bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            LOG(INFO) << "Not bind. Retrying...";
            continue;
        }

        return servinfo; // if we get here, we must have connected successfully
    }

    LOG(ERROR) << "Failed to bind socket";
    return NULL;

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
        receiveMsg(senderSockfd);

    }
    pthread_exit(NULL);
}

void NetworkController::receiveMsg(int senderSockfd) {
    char *buffer = new char[1000];
    int msgLength;
    while (msgLength >= 128) {
        msgLength = read(senderSockfd, buffer, 128);
    }
    close(senderSockfd);
    //TODO dodać do kolejki
    std::shared_ptr<MessageWrapper> msg;
    receiveQueue->push(msg);
}


void NetworkController::stop() {
    pthread_cancel(*sendSystemThread);
    pthread_cancel(*receiveSystemThread);
}
