//
// Created by hubert on 03.12.15.
//
#include <string>
#include "gtest/gtest.h"
#include "Queue.hpp"

TEST(Queue, creating_test) {
    Queue<int> *queue = new Queue<int>();
    queue->push(5);

    EXPECT_TRUE(queue->pop() == 5);
}

