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


class NetworkController {

private:
    Queue<std::pair<SimpleMessage, struct sockaddr *>> *sendQueue;
    Queue<SimpleMessage> *receiveQueue;
    int receiveSockfd;
    int sendSockfd;
    const sockaddr *localAddress;

    void prepareReceiveThread();

    void createReceiveThread();

    void prepareSendThread();

    void createSendThread();

    bool sendMsg(const char *msg, struct sockaddr *address);

    void receiveMsg(struct sockaddr *senderAddress, int senderSockfd);

    static void *startReceiveThread(void *param);

    static void *startSendThread(void *param);

public:
    NetworkController() { };

    NetworkController(Queue<std::pair<SimpleMessage, struct sockaddr *>> *sendQueue, Queue<SimpleMessage> *receiveQueue,
                      const sockaddr *localAddress)
            : sendQueue(sendQueue),
              receiveQueue(receiveQueue),
              sendSockfd(0), receiveSockfd(0),
              localAddress(localAddress) {
        prepareReceiveThread();
        prepareSendThread();
    };

};


#endif //RING_ABSTRACTNETWORKCONTROLLER_H
