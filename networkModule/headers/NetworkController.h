//
// Created by hubert on 03.12.15.
//

#ifndef RING_NETWORKCONTROLLER_H
#define RING_NETWORKCONTROLLER_H

#include <sys/types.h>
#include <sys/socket.h>


class NetworkController {

private:
    Queue sendQueue;
    Queue receiveQueue;

public:
    NetworkController(const Queue &sendQueue, const Queue &receiveQueue) : sendQueue(sendQueue),
                                                                           receiveQueue(receiveQueue) { };
    

};


#endif //RING_NETWORKCONTROLLER_H
