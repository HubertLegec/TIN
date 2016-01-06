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
#include "../networkMessage/headers/CategoryListMessage.h"

INITIALIZE_EASYLOGGINGPP
TEST(Queue, creating_test) {
    Queue<int> *queue = new Queue<int>();
    queue->push(5);
    Queue<std::shared_ptr<MessageWrapper>> sQueue;
    Queue<std::shared_ptr<SimpleMessage>> rQueue;
//    SimpleMessage msg(MessageType::DEACTIVATE_CATEGORY, 1);
//    UserManagementMessage msg(MessageType::CREATE_USER_ACCOUNT, "127.0.1.1", 3499, "Simple category");

    CategoryListMessage *msg = new CategoryListMessage();
    msg->addCategory(0, "Dzik");
    msg->setType(MessageType::CATEGORY_LIST);

    std::shared_ptr<CategoryListMessage> p2(msg);
    MessageWrapper *wrapper = new MessageWrapper(p2, "127.0.1.1", 3499);
    Queue<std::shared_ptr<MessageWrapper>> sQueue1;
    std::shared_ptr<MessageWrapper> p1(wrapper);
//    sQueue.push(p1);
    sQueue.push(p1);
    Queue<std::shared_ptr<SimpleMessage>> rQueue1;
    NetworkController(&sQueue1, &rQueue1, "129.0.1.1", 3499);
//    NetworkController(&sQueue, &rQueue1, 3498);
    sleep(9);
    NetworkController(&sQueue, &rQueue, "129.0.1.1", 3491);
    sleep(3);
    EXPECT_TRUE(true);
    sleep(3);
}

