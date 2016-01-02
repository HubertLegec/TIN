//
// Created by hubert on 03.12.15.
//

#include <unistd.h>
#include "../headers/NetworkController.h"
#include "../../logger/easylogging++.h"


void NetworkController::prepareSendThread() {
    LOG(INFO) << "[SND] Start sending thread";
    createThread(sendSystemThread, startSendThread);
}

void NetworkController::createThread(pthread_t *thread, void *(*function)(void *)) {
//    int rc;
    pthread_create(thread, NULL, function, ((void *) pointer));
//    LOG(INFO) << "Joining thread";
//    rc = pthread_join(*thread, NULL);
//    if (rc) {
//        LOG(ERROR) << "Unable to join thread" << rc;
//        exit(-1);
//    }
}

void *NetworkController::startSendThread(void *param) {
    NetworkController *obj = (NetworkController *) param;
    obj->createSendThread();
}

void NetworkController::createSendThread() {
    LOG(INFO) << "[SND] Starting process of sending messages";
    while (true) {
        std::shared_ptr<MessageWrapper> msg = sendQueue->pop();
        LOG(INFO) << "[SND] I get msg to send: " << msg->getMessage()->toString();
        prepareConncetionWithReceiver(msg);
        if (!sendMsg(msg->getMessage())) {
            LOG(ERROR) << "[SND] Couldnt send msg";
            break;
        }
    }
    pthread_exit(NULL);
}

struct addrinfo *NetworkController::prepareConncetionWithReceiver(std::shared_ptr<MessageWrapper> msg) {
    LOG(INFO) << "[SND] Creating connection with receiver. Hostname: " << msg->getIP() << " Port: " << msg->getPort();

    hostent *hp;
    struct in_addr ipv4addr;
    struct in6_addr ipv6addr;
    sockaddr_in receiver, client;

    inet_pton(AF_INET, msg->getIP().c_str(), &ipv4addr);
    hp = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
    LOG(INFO) << "[SND] Receiver running at host NAME: " << hp->h_name;
    bcopy(hp->h_addr, &(receiver.sin_addr), hp->h_length);
    LOG(INFO) << "[SND] Receiver INET ADDRESS is: " << inet_ntoa(receiver.sin_addr);

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(msg->getPort());

    sendSockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sendSockfd, (struct sockaddr *) &client, sizeof(client));

    connect(sendSockfd, (struct sockaddr *) &receiver, sizeof(receiver));
    sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    getpeername(sendSockfd, (struct sockaddr *) &from, &fromlen);
    LOG(INFO) << "[SND] Connected to TCPServer1: " << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port);
    hp = gethostbyaddr((char *) &from.sin_addr.s_addr,
                       sizeof(from.sin_addr.s_addr), AF_INET);
    LOG(INFO) << "[SND] Name is : " << hp->h_name;

}

bool NetworkController::sendMsg(std::shared_ptr<SimpleMessage> msg) {
    //TODO do dorobienia obsługa wysyłania niepełnej wiadomości
    const char *serializedMsg = serializeMsg(msg);
    int len = strlen(serializedMsg);
    int sentBytes = 0;
    LOG(INFO) << "[SND] Sending msg: " << serializedMsg << " with length: " << len << std::endl;
    LOG(INFO) << "[SND] msg: " << msg->toString();
    sentBytes = send(sendSockfd, serializedMsg, len, 0);
//    sentBytes = write(sendSockfd, serializedMsg, len);
    LOG(INFO) << "[SND] Sent: " << sentBytes << " bytes";
    int trialCounter = 1;
    if (sentBytes == len) {
        while (trialCounter < 4 && close(sendSockfd) != 0) {
            LOG(INFO) << "[SND] Something went wrong during closing connection. Retrying..." << trialCounter++;
        }
    }
    if (trialCounter > 3) {
        LOG(ERROR) << "[SND] Something went wrong during closing connection.";
        return false;
    }
    return true;
}

const char *NetworkController::serializeMsg(std::shared_ptr<SimpleMessage> msg) {
    std::stringstream ss;
    cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
    oarchive(*msg); // Write the data to the archive
    return getcharFromString(ss.str());

}

const char *NetworkController::getcharFromString(std::string string) {
    char *result = new char[string.length()];
    for (int i = 0; i < string.length(); ++i) {
        result[i] = '\0';
    }
    int j = 0;
    for (int i = 0; i < string.length(); ++i) {
        if (((int) string[i]) != 0) {
            result[j++] = string[i];
        }
    }
    return result;
}

void NetworkController::prepareReceiveThread() {
    LOG(INFO) << "[REC] Starting preparing receive thread";
    prepareListeningSocket();
    //TODO drugi parametr - liczba połączeń oczekujących, do ogarnięcia
    LOG(INFO) << "[REC] Listen  with sockfd: " << receiveSockfd;
    listen(receiveSockfd, 10000);
    pthread_t thread;
    receiveSystemThread = &thread;
    createThread(receiveSystemThread, startReceiveThread);
}

void NetworkController::prepareListeningSocket() {
    LOG(INFO) << "[REC] Starting preparing socket";
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    LOG(INFO) << "[REC] Getting my own address";
    struct sockaddr_in server;
    hostent *hp;
    char hostname[128];
    gethostname(hostname, sizeof(hostname));
    LOG(INFO) << "[REC] TCP/Server running at host NAME: " << hostname;
    hp = gethostbyname(hostname);
    bcopy(hp->h_addr, &(server.sin_addr), hp->h_length);
    LOG(INFO) << "[REC] TCP/Server INET ADDRESS is: " << inet_ntoa(server.sin_addr);

    LOG(INFO) << "[REC] Trying to get socket";
    if ((receiveSockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        LOG(ERROR) << "[REC] Problem with creating socket";
//            continue;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(myPort));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    int length = sizeof(server);
    if (bind(receiveSockfd, (struct sockaddr *) &server, length) == -1) {
        close(receiveSockfd);
        LOG(ERROR) << "[REC] Not bind.";
//            continue;
    }
    socklen_t length1 = sizeof(server);
    getsockname(receiveSockfd, (struct sockaddr *) &server, &length1);
    LOG(INFO) << "[REC] Server Port is: " << ntohs(server.sin_port);


}

void *NetworkController::startReceiveThread(void *param) {
    LOG(INFO) << "[REC] Start receive thread";
    NetworkController *obj = (NetworkController *) param;
    obj->createReceiveThread();
    pthread_exit(NULL);

}

void NetworkController::createReceiveThread() {
    LOG(INFO) << "[REC] Receiving connection with sockfd: " << receiveSockfd;
    //TODO do obsłużenia mechanizm zamykania wątków
    while (true) {

        struct sockaddr_in peer_name;
        socklen_t socklen = sizeof(peer_name);
        socklen_t *senderAddresLen;
        int senderSockfd = accept(receiveSockfd, (struct sockaddr *) &peer_name, &socklen);
        if (senderSockfd == -1) {
            //TODO błąd do obsłużenia
            LOG(INFO) << "[REC] Error during accept connection. Retrying... ";
            continue;
        }
        LOG(INFO) << "[REC] Aceepted connection from sockfd: " << senderSockfd;
        LOG(INFO) << "[REC] I get correct msg. Processing...";
        receiveMsg(senderSockfd, peer_name);

    }
//    pthread_exit(NULL);
}


void NetworkController::receiveMsg(int senderSockfd, struct sockaddr_in from) {
//    LOG(INFO) << "Serving " << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port);
    hostent *hp;
    hp = gethostbyaddr((char *) &from.sin_addr.s_addr, sizeof(from.sin_addr.s_addr), AF_INET);
    LOG(INFO) << "[REC] Name is : " << hp->h_name;
    char *buffer = new char[1000];
    int msgLength = 0;
    int i = 0;
    while ((msgLength = recv(senderSockfd, buffer, sizeof(buffer), 0)) > 0) {
        i += msgLength;
    }
    LOG(INFO) << "[REC] I get: " << i << " bytes";
//    buffer[i] = NULL;
    LOG(INFO) << "[REC] Msg before transform: " << buffer;
    close(senderSockfd);
    //TODO dodać do kolejki
    SimpleMessage receivedMsg;
    LOG(INFO) << "Start reserialize";
    std::stringstream ss(getStringFromChar(i, buffer));
    cereal::BinaryInputArchive iarchive(ss); // Create an input archive
    iarchive(receivedMsg);
    std::shared_ptr<SimpleMessage> msg(&receivedMsg);
    LOG(INFO) << "[REC] MSG: " << msg->toString();
    receiveQueue->push(msg);
}

std::string NetworkController::getStringFromChar(int length, const char *tab) {
    int size = ((length / 16) + 1) * 16;
    LOG(INFO) << "[REC] length: " << size;
    std::string result;
    result.resize(size);
    result[0] = tab[0];
    for (int i = 1; i < size; ++i) {
        if ((i % 4) == 0 && (i / 4) < length) {
            result[(i / 4) * 4] = tab[i / 4];
        }
        else {
            result[i] = 0;
        }
    }
    return result;
}


void NetworkController::stop() {
    pthread_cancel(*sendSystemThread);
    pthread_cancel(*receiveSystemThread);
}
