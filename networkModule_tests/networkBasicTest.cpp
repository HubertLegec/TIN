//
// Created by hubert on 03.12.15.
//
#include <string>
#include "gtest/gtest.h"
#include "../utils/Queue.hpp"
#include "../networkModule/headers/NetworkController.h"
#include "../logger/easylogging++.h"

INITIALIZE_EASYLOGGINGPP
TEST(Queue, creating_test) {
    Queue<int> *queue = new Queue<int>();
    queue->push(5);
    Queue<std::shared_ptr<MessageWrapper>> sQueue;
    Queue<std::shared_ptr<SimpleMessage>> rQueue;
    SimpleMessage msg(MessageType::SERVER_INFO, 1);
    std::shared_ptr<SimpleMessage> p2(&msg);
    MessageWrapper *wrapper = new MessageWrapper(p2, "127.0.1.1", 3491);
    Queue<std::shared_ptr<MessageWrapper>> sQueue1;
    std::shared_ptr<MessageWrapper> p1(wrapper);
//    sQueue1.push(p1);
    sQueue.push(p1);
    Queue<std::shared_ptr<SimpleMessage>> rQueue1;
    NetworkController(&sQueue1, &rQueue1, "127.0.1.1", 3499);
    sleep(9);
    NetworkController(&sQueue, &rQueue, "127.0.1.1", 3491);
    sleep(99);
    EXPECT_TRUE(queue->pop() == 5);
}

