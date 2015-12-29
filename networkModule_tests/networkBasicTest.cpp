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
    Queue<std::shared_ptr<MessageWrapper>> rQueue;
    SimpleMessage msg(MessageType::SERVER_INFO, 1);
    std::shared_ptr<SimpleMessage> p2(&msg);
    MessageWrapper *wrapper = new MessageWrapper(p2, "localhost", 3491);
    std::shared_ptr<MessageWrapper> p1(wrapper);
    sQueue.push(p1);
    Queue<std::shared_ptr<MessageWrapper>> sQueue1;
    Queue<std::shared_ptr<MessageWrapper>> rQueue1;
    NetworkController(&sQueue, &rQueue, "localhost", 3491);
//    NetworkController(&sQueue1, &rQueue1, "localhost", 3490);
    EXPECT_TRUE(queue->pop() == 5);
}

