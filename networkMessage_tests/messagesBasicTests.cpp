//
// Created by hubert on 05.12.15.
//

#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "../networkMessage/headers/CategoryListMessage.h"
#include "../networkMessage/headers/GetMessage.h"
#include "../networkMessage/headers/ServerInfoMessage.h"
#include "../networkMessage/headers/CategoryManagementMessage.h"
#include "../networkMessage/headers/NeighboursInfoMessage.h"
#include "../networkMessage/headers/RingMessage.h"

TEST(CategoryListMessage, creating_test){
    std::vector<std::string> names = {"First", "Second", "Third"};


    CategoryListMessage msg(1, names);

    ASSERT_EQ(MessageType::CATEGORY_LIST, msg.getMessageType());
    ASSERT_EQ(45, msg.getMessageSize());
    ASSERT_EQ(3, msg.getCategoryList().size());
    ASSERT_EQ(1, msg.getSenderID());
    for(int i = 0; i < 3; i++){
        EXPECT_TRUE( names[i].compare(msg.getCategoryList()[i]) == 0);
    }
}

TEST(GetMessage, creating_test){
    GetMessage msg(1, GetMessageType::CAT_LIST);

    ASSERT_EQ(MessageType::GET, msg.getMessageType());
    ASSERT_EQ(GetMessageType::CAT_LIST, msg.getRequestType());
    ASSERT_EQ(1, msg.getSenderID());
}

TEST(ServerInfoMessage, creating_test){
    ServerInfoMessage msg(1, ServerInfoMessageType::OK, "Everything OK");

    ASSERT_EQ(MessageType::SERVER_INFO, msg.getMessageType());
    ASSERT_EQ(28+14, msg.getMessageSize());
    EXPECT_TRUE(msg.getInfo().compare("Everything OK") == 0);
    EXPECT_EQ(-1, msg.getExtraInfo());

    msg.setExtraInfo(111);
    EXPECT_EQ(111, msg.getExtraInfo());

    msg.setInfo("Another info");
    EXPECT_TRUE(msg.getInfo().compare("Another info") == 0);
}

TEST(CategoryManagementMessage, creating_test){
    CategoryManagementMessage msg(1, MessageType::CREATE_CATEGORY, "Simple Category", "Bob");

    ASSERT_EQ(MessageType::CREATE_CATEGORY, msg.getMessageType());
    ASSERT_EQ(44, msg.getMessageSize());
    EXPECT_TRUE(msg.getCategoryName().compare("Simple Category") == 0);
    EXPECT_TRUE(msg.getUserName().compare("Bob") == 0);
}

TEST(NeighboursInfoMessage, creating_test){
    NeighboursInfoMessage msg("Mathew", "John");

    ASSERT_EQ(MessageType::NEIGHBOURS_SET, msg.getMessageType());
    ASSERT_EQ(36, msg.getMessageSize());
    ASSERT_EQ(0, msg.getSenderID());
    EXPECT_TRUE( msg.getLeftNeighbourName().compare("Mathew") == 0);
    EXPECT_TRUE( msg.getRightNeighbourName().compare("John") == 0);
}

TEST(RingMessage, creating_test){
    RingMessage msg(1, "Message text");

    ASSERT_EQ(MessageType::RING_MESSAGE, msg.getMessageType());
    ASSERT_EQ(45, msg.getMessageSize());
    ASSERT_EQ(1, msg.getSenderID());
    EXPECT_TRUE(msg.getMsgText().compare("Message text") == 0);
    EXPECT_EQ(0, msg.getConfirmationsList().size());

    msg.addConfirmation("Ann");

    EXPECT_EQ(1, msg.getConfirmationsList().size());
    EXPECT_TRUE(msg.getConfirmationsList()[0].compare("Ann") == 0);

    std::vector<std::string> names = {"Tom", "Kate"};
    msg.addConfirmations(names);

    EXPECT_EQ(3, msg.getConfirmationsList().size());
    EXPECT_TRUE(msg.getConfirmationsList()[0].compare("Ann") == 0);
    EXPECT_TRUE(msg.getConfirmationsList()[1].compare("Tom") == 0);
    EXPECT_TRUE(msg.getConfirmationsList()[2].compare("Kate") == 0);
}
