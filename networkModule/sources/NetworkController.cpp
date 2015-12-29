//
// Created by hubert on 03.12.15.
//

#include <unistd.h>
#include "../headers/NetworkController.h"
#include "../../logger/easylogging++.h"


void NetworkController::prepareSendThread() {
    LOG(INFO) << "Preparing Send thread";
    createThread(sendSystemThread, startSendThread);
}

void NetworkController::createThread(pthread_t *thread, void *(*function)(void *)) {
    LOG(INFO) << "Creating thread for " << function;
    int rc;
    pthread_create(thread, NULL, function, ((void *) pointer));
//    LOG(INFO) << "Joining thread";
//    rc = pthread_join(*thread, NULL);
//    if (rc) {
//        LOG(ERROR) << "Unable to join thread" << rc;
//        exit(-1);
//    }
}

void *NetworkController::startSendThread(void *param) {
    LOG(INFO) << "function: startSendThread";
    NetworkController *obj = (NetworkController *) param;
    obj->createSendThread();
}

void NetworkController::createSendThread() {
    LOG(INFO) << "Starting sending messages";
    while (true) {
        std::shared_ptr<MessageWrapper> msg = sendQueue->pop();
        LOG(INFO) << "I get msg to send: " << msg->getMessage()->toString();
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
    LOG(INFO) << "Creating connection with receiver. Hostname: " << msg->getIP() << " Port: " << msg->getPort();
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    const char *hostname = msg->getIP().c_str();
    std::string s = std::to_string(msg->getPort());
    char const *port = s.c_str();
    if ((getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        LOG(ERROR) << "Didnt find address with hostname: " << hostname << " port: " << port;
//        exit(1);
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
    while (trialCounter < 40000 && close(sendSockfd) != 0) {
//        LOG(INFO) << "Something went wrong during closing connection. Retrying..." << trialCounter++;
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
    LOG(INFO) << "Starting preparing receive thread";
    if (prepareListeningSocket() == NULL)
        exit(1);
    //TODO drugi parametr - liczba połączeń oczekujących, do ogarnięcia
    LOG(INFO) << "Listen ";
    listen(receiveSockfd, 10);
    pthread_t thread;
    receiveSystemThread = &thread;
    createThread(receiveSystemThread, startReceiveThread);
}

struct addrinfo *NetworkController::prepareListeningSocket() {
    LOG(INFO) << "Starting preparing socket";
    int sockfd;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP address
    LOG(INFO) << "Getting my own address";
    if ((getaddrinfo(NULL, myPort, &hints, &servinfo)) != 0) {
        LOG(ERROR) << "Didnt find own address with hostname: " << myIP << " port: " << myPort;
        //TODO obsługa
//        exit(1);
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        LOG(INFO) << "Searching my own address";
        LOG(INFO) << "Trying to get socket";
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            LOG(INFO) << "Not created socket. Retrying...";
            continue;
        }
        LOG(INFO) << "Trying to bind";
        if ((receiveSockfd = bind(sockfd, p->ai_addr, p->ai_addrlen)) == -1) {
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
    LOG(INFO) << "Start receive thread";
    NetworkController *obj = (NetworkController *) param;
    LOG(INFO) << "Start function";
    obj->createReceiveThread();
    pthread_exit(NULL);

}

void NetworkController::createReceiveThread() {
    LOG(INFO) << "Receiving connection";
    //TODO do obsłużenia mechanizm zamykania wątków
    while (true) {

        struct sockaddr peer_name;
        int socklen = sizeof(peer_name);
        socklen_t *senderAddresLen;
//        LOG(INFO) << "waitning for accept" << receiveSockfd;
        int senderSockfd = accept(receiveSockfd, &peer_name, (socklen_t *) &socklen);
//        LOG(INFO) << "Aceepted connection" << senderSockfd;
        if (senderSockfd == -1) {
            //TODO błąd do obsłużenia
//            LOG(INFO) << "I couldnt connect with receiver";
            continue;
//            break;
        }
        LOG(INFO) << "I get correct msg. Processing...";
        int currentSenderPort = 0;
        char *hostname = getIpAndAddress(&peer_name, hostname, 100 * sizeof(char), currentSenderPort);
        std::string currentSenderHostname(hostname);
        receiveMsg(senderSockfd, currentSenderHostname, currentSenderPort);

    }
//    pthread_exit(NULL);
}

char *NetworkController::getIpAndAddress(const struct sockaddr *sa, char *s, size_t maxlen, int &port) {
    switch (sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *) sa)->sin_addr),
                      s, maxlen);
            port = ((struct sockaddr_in *) sa)->sin_port;
            break;
        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *) sa)->sin6_addr),
                      s, maxlen);
            port = ((struct sockaddr_in6 *) sa)->sin6_port;
            break;
        default:
            strncpy(s, "Unknown AF", maxlen);
            return NULL;
    }

    return s;
}

void NetworkController::receiveMsg(int senderSockfd, std::string hostname, int port) {
    char *buffer = new char[1000];
    int msgLength = 0;
    int i = 0;
    while ((msgLength = read(senderSockfd, buffer + i, 128)) > 0) {
        i += msgLength;
    }
    LOG(INFO) << "MSG: " << buffer;
    close(senderSockfd);
    //TODO dodać do kolejki
    SimpleMessage receivedMsg;
    std::stringstream ss(buffer);
    cereal::BinaryInputArchive iarchive(ss); // Create an input archive
    iarchive(receivedMsg);
    std::shared_ptr<SimpleMessage> msg(&receivedMsg);
    std::string string("");
    MessageWrapper wrapMsg(msg, hostname, port);
    std::shared_ptr<MessageWrapper> wrappedMsg(&wrapMsg);
    receiveQueue->push(wrappedMsg);
}


void NetworkController::stop() {
    pthread_cancel(*sendSystemThread);
    pthread_cancel(*receiveSystemThread);
}
