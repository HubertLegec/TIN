//
// Created by hubert on 03.12.15.
//
#include <string>
#include "gtest/gtest.h"
#include "../utils/Queue.hpp"
#include "../networkModule/headers/NetworkController.h"
#include "../logger/easylogging++.h"
#include "../networkMessage/headers/UserManagementMessage.h"
#include "../networkMessage/headers/SimpleMessage.h"

INITIALIZE_EASYLOGGINGPP
TEST(Queue, creating_test) {
    Queue<int> *queue = new Queue<int>();
    queue->push(5);
    Queue<std::shared_ptr<MessageWrapper>> sQueue;
    Queue<std::shared_ptr<SimpleMessage>> rQueue;
//    SimpleMessage msg(MessageType::DEACTIVATE_CATEGORY, 1);
    UserManagementMessage msg(MessageType::CREATE_USER_ACCOUNT, "127.0.1.1", 3499, "Simple category");
    std::shared_ptr<SimpleMessage> p2(&msg);
    MessageWrapper *wrapper = new MessageWrapper(p2, "127.0.1.1", 3499);
    Queue<std::shared_ptr<MessageWrapper>> sQueue1;
    std::shared_ptr<MessageWrapper> p1(wrapper);
//    sQueue.push(p1);
    sQueue.push(p1);
    Queue<std::shared_ptr<SimpleMessage>> rQueue1;
    NetworkController(&sQueue1, &rQueue1, 3499);
    sleep(9);
    NetworkController(&sQueue, &rQueue, 3491);
    sleep(3);
    EXPECT_TRUE(rQueue1.pop() == p2);
    sleep(3);
}

