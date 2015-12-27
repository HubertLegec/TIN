//
// Created by hubert on 03.12.15.
//
#include <string>
#include "gtest/gtest.h"
#include "../utils/Queue.hpp"
#include "../networkModule/headers/NetworkController.h"

TEST(Queue, creating_test) {
    Queue<int> *queue = new Queue<int>();
    queue->push(5);
    NetworkController(NULL, NULL, NULL);
    EXPECT_TRUE(queue->pop() == 5);
}

