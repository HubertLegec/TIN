//
// Created by hubert on 03.12.15.
//

#ifndef RING_ABSTRACTNETWORKCONTROLLER_H
#define RING_ABSTRACTNETWORKCONTROLLER_H

#include <sys/types.h>
#include <sys/socket.h>
#include "../../networkMessage/headers/SimpleMessage.h"
#include "../../networkMessage/headers/NetworkControllerErrorMessage.h"
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../../utils/Queue.hpp"
#include "MessageWrapper.h"
#include <unistd.h>
#include <mutex>
#include <condition_variable>


class NetworkController {

private:
    Queue<std::shared_ptr<MessageWrapper>> *sendQueue;
    Queue<std::shared_ptr<SimpleMessage>> *receiveQueue;
    int receiveSockfd;
    int sendSockfd;
    sockaddr_in myAddr;
    pthread_t *sendSystemThread;
    pthread_t *receiveSystemThread;
    struct addrinfo *ownAddress;
    const char *myIP;
    const char *myPort;
    NetworkController *pointer;
    std::mutex mutex_;
    bool exitFlag;

    void prepareReceiveThread();

    void createReceiveThread();

    void prepareSendThread();

    void createSendThread();

    bool sendMsg(std::shared_ptr<SimpleMessage> msg);

    void receiveMsg(int senderSockfd, struct sockaddr_in from);

    void createThread(pthread_t *thread, void *(*function)(void *));

    bool prepareConncetionWithReceiver(std::shared_ptr<MessageWrapper> msg);

    std::string getStringFromChar(int length, const char *tab);

    const char *getcharFromString(std::string string);

    void prepareListeningSocket();

    const char *serializeMsg(std::shared_ptr<SimpleMessage> msg, int &length);

    static void *startReceiveThread(void *param);

    static void *startSendThread(void *param);

    std::shared_ptr<SimpleMessage> prepareErrorMsg(NetworkControllerErrorMessage::ErrorCode errorCode,
                                                   std::string info);

public:
    NetworkController() { };

    NetworkController(Queue<std::shared_ptr<MessageWrapper>> *sendQueue,
                      Queue<std::shared_ptr<SimpleMessage>> *receiveQueue, int port)
            : sendQueue(sendQueue),
              receiveQueue(receiveQueue),
              sendSockfd(0), receiveSockfd(0) {
        exitFlag = false;
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
