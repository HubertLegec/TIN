//
// Created by hubert on 03.12.15.
//

#ifndef RING_ABSTRACTNETWORKCONTROLLER_H
#define RING_ABSTRACTNETWORKCONTROLLER_H

#include <sys/types.h>
#include <sys/socket.h>
#include "../../networkMessage/headers/SimpleMessage.h"
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../../utils/Queue.hpp"
#include "MessageWrapper.h"


class NetworkController {

private:
    Queue<std::shared_ptr<MessageWrapper>> *sendQueue;
    Queue<std::shared_ptr<MessageWrapper>> *receiveQueue;
    int receiveSockfd;
    int sendSockfd;
    pthread_t *sendSystemThread;
    pthread_t *receiveSystemThread;
    struct addrinfo *ownAddress;
//    const sockaddr *localAddress;
    const char *myIP;
    const char *myPort;
    NetworkController *pointer;

    void prepareReceiveThread();

    void createReceiveThread();

    void prepareSendThread();

    void createSendThread();

    bool sendMsg(std::shared_ptr<SimpleMessage> msg);

    void receiveMsg(int senderSockfd, std::string hostname, int port);

    void createThread(pthread_t *thread, void *(*function)(void *));

    char *getIpAndAddress(const struct sockaddr *sa, char *s, size_t maxlen, int &port);

    struct addrinfo *prepareConncetionWithReceiver(std::shared_ptr<MessageWrapper> msg);

    struct addrinfo *prepareListeningSocket();

    const char *serializeMsg(std::shared_ptr<SimpleMessage> msg);

    static void *startReceiveThread(void *param);

    static void *startSendThread(void *param);

public:
    NetworkController() { };

    NetworkController(Queue<std::shared_ptr<MessageWrapper>> *sendQueue,
                      Queue<std::shared_ptr<MessageWrapper>> *receiveQueue, std::string ip, int port)
            : sendQueue(sendQueue),
              receiveQueue(receiveQueue),
              sendSockfd(0), receiveSockfd(0) {
        myIP = ip.c_str();
        std::string s = std::to_string(port);
        myPort = s.c_str();
        pointer = this;
        pthread_t sThread, rThread;
        sendSystemThread = &sThread;
        receiveSystemThread = &rThread;
        prepareReceiveThread();
        prepareSendThread();
    };

    //close all sockets and stop all threads
    void stop();

};


#endif //RING_ABSTRACTNETWORKCONTROLLER_H
