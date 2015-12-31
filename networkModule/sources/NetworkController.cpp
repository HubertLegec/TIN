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
        prepareConncetionWithReceiver(msg);
        if (!sendMsg(msg->getMessage())) {
            LOG(ERROR) << "Couldnt send msg";
            break;
        }
    }
    pthread_exit(NULL);
}

struct addrinfo *NetworkController::prepareConncetionWithReceiver(std::shared_ptr<MessageWrapper> msg) {
    LOG(INFO) << "Creating connection with receiver. Hostname: " << msg->getIP() << " Port: " << msg->getPort();

    hostent *hp;
    struct in_addr ipv4addr;
    struct in6_addr ipv6addr;
    sockaddr_in receiver, client;

    inet_pton(AF_INET, msg->getIP().c_str(), &ipv4addr);
    hp = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
    LOG(INFO) << "Receiver running at host NAME: " << hp->h_name;
    bcopy(hp->h_addr, &(receiver.sin_addr), hp->h_length);
    LOG(INFO) << "Receiver INET ADDRESS is: " << inet_ntoa(receiver.sin_addr);

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(msg->getPort());

    sendSockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sendSockfd, (struct sockaddr *) &client, sizeof(client));

    connect(sendSockfd, (struct sockaddr *) &receiver, sizeof(receiver));
    sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    getpeername(sendSockfd, (struct sockaddr *) &from, &fromlen);
    LOG(INFO) << "Connected to TCPServer1: ";
    LOG(INFO) << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port);
    hp = gethostbyaddr((char *) &from.sin_addr.s_addr,
                       sizeof(from.sin_addr.s_addr), AF_INET);
    LOG(INFO) << "Name is : " << hp->h_name;

}

bool NetworkController::sendMsg(std::shared_ptr<SimpleMessage> msg) {
    //TODO do dorobienia obsługa wysyłania niepełnej wiadomości
    const char *serializedMsg = serializeMsg(msg);
    int len = strlen(serializedMsg);
    int sentBytes = 0;
    LOG(INFO) << "Sending msg: " << serializedMsg << " with length: " << len << std::endl;
    LOG(INFO) << "msg: " << msg->toString();
    sentBytes = send(sendSockfd, serializedMsg, len, 0);
//    sentBytes = write(sendSockfd, serializedMsg, len);
    LOG(INFO) << "Sent: " << sentBytes << " bytes";
    if (sentBytes == len) {
        sleep(1);
        close(sendSockfd);
    }
    int trialCounter = 1;
//    while (trialCounter < 40000 && close(sendSockfd) != 0) {
//        LOG(INFO) << "Something went wrong during closing connection. Retrying..." << trialCounter++;
//    }
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
    LOG(INFO) << "Starting preparing receive thread";
    if (prepareListeningSocket() == NULL)
//        exit(1);
        //TODO drugi parametr - liczba połączeń oczekujących, do ogarnięcia
        LOG(INFO) << "Listen  with sockfd: " << receiveSockfd;
    listen(receiveSockfd, 10000);
    pthread_t thread;
    receiveSystemThread = &thread;
    createThread(receiveSystemThread, startReceiveThread);
}

struct addrinfo *NetworkController::prepareListeningSocket() {
    LOG(INFO) << "Starting preparing socket";
    int sockfd;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(struct addrinfo));
    LOG(INFO) << "Getting my own address";
    struct sockaddr_in server;
    hostent *hp;
    char hostname[128];
    gethostname(hostname, sizeof(hostname));
    printf("----TCP/Server running at host NAME: %s\n", hostname);
    hp = gethostbyname(hostname);
    bcopy(hp->h_addr, &(server.sin_addr), hp->h_length);
    printf("    (TCP/Server INET ADDRESS is: %s )\n", inet_ntoa(server.sin_addr));

    LOG(INFO) << "Searching my own address";
    LOG(INFO) << "Trying to get socket";
    if ((receiveSockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        LOG(INFO) << "Not created socket. Retrying...";
//            continue;
    }
    LOG(INFO) << "Trying to bind";
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(myPort));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    int length = sizeof(server);
    if (bind(receiveSockfd, (struct sockaddr *) &server, length) == -1) {
        close(receiveSockfd);
        LOG(INFO) << "Not bind. Retrying...";
//            continue;
    }
    socklen_t length1 = sizeof(server);
    getsockname(receiveSockfd, (struct sockaddr *) &server, &length1);
    printf("Server Port is: %d\n", ntohs(server.sin_port));
//        break; // if we get here, we must have connected successfully
//    }



}

void *NetworkController::startReceiveThread(void *param) {
    LOG(INFO) << "Start receive thread";
    NetworkController *obj = (NetworkController *) param;
    LOG(INFO) << "Start function";
    obj->createReceiveThread();
    pthread_exit(NULL);

}

void NetworkController::createReceiveThread() {
    LOG(INFO) << "Receiving connection with sockfd: " << receiveSockfd;
    //TODO do obsłużenia mechanizm zamykania wątków
    while (true) {

        struct sockaddr_in peer_name;
        socklen_t socklen = sizeof(peer_name);
        socklen_t *senderAddresLen;
        int senderSockfd = accept(receiveSockfd, (struct sockaddr *) &peer_name, &socklen);
        if (senderSockfd == -1) {
            //TODO błąd do obsłużenia
//            LOG(INFO) << "I couldnt connect with receiver";
            continue;
//            break;
        }
        LOG(INFO) << "Aceepted connection" << senderSockfd;
        LOG(INFO) << "I get correct msg. Processing...";
        int currentSenderPort = 0;
//        char *hostname = getIpAndAddress(&peer_name, hostname, 100 * sizeof(char), currentSenderPort);
//        std::string currentSenderHostname(hostname);
        receiveMsg(senderSockfd, peer_name);

    }
//    pthread_exit(NULL);
}


void NetworkController::receiveMsg(int senderSockfd, struct sockaddr_in from) {
    LOG(INFO) << "Serving " << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port);
    hostent *hp;
    hp = gethostbyaddr((char *) &from.sin_addr.s_addr, sizeof(from.sin_addr.s_addr), AF_INET);
    LOG(INFO) << "Name is : " << hp->h_name;
    char *buffer = new char[1000];
    int msgLength = 0;
    int i = 0;
    while ((msgLength = recv(senderSockfd, buffer, sizeof(buffer), 0)) > 0) {

        i += msgLength;
    }
//    buffer[i] = NULL;
    LOG(INFO) << "MSG: " << buffer;
    close(senderSockfd);
    //TODO dodać do kolejki
    SimpleMessage receivedMsg;
    LOG(INFO) << "Start reserialize";
    std::stringstream ss(getStringFromChar(buffer));
    cereal::BinaryInputArchive iarchive(ss); // Create an input archive
    iarchive(receivedMsg);
    std::shared_ptr<SimpleMessage> msg(&receivedMsg);
    LOG(INFO) << "Succes, i get msg: " << msg->toString();
    receiveQueue->push(msg);
}

std::string NetworkController::getStringFromChar(const char *tab) {
    int length = 0;
    while (true) {
        if (tab[length++] == '\0')
            break;
    }
    length = ((length / 16) + 1) * 16;
    std::string result;
    result.resize(16);
    result[0] = tab[0];
    for (int i = 1; i < length; ++i) {
        if ((i % 4) == 0) {
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
