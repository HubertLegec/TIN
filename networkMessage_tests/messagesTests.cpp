//
// Created by hubert on 05.12.15.
//

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "gtest/gtest.h"
#include "../networkMessage/headers/CategoryListMessage.h"
#include "../networkMessage/headers/GetMessage.h"
#include "../networkMessage/headers/ServerInfoMessage.h"
#include "../networkMessage/headers/CategoryManagementMessage.h"
#include "../networkMessage/headers/NeighboursInfoMessage.h"
#include "../networkMessage/headers/RingMessage.h"

TEST(CategoryListMessage, creating_test){
    std::map<long, std::string> names;
    names.insert(std::pair<long, std::string>(1, "First"));
    names.insert(std::pair<long, std::string>(2, "Second"));
    names.insert(std::pair<long, std::string>(3, "Third"));


    CategoryListMessage msg(1, names);

    ASSERT_EQ(MessageType::CATEGORY_LIST, msg.getMessageType());
    ASSERT_EQ(64, msg.getMessageSize());
    ASSERT_EQ(3, msg.getCategories().size());
    ASSERT_EQ(1, msg.getSenderID());
    EXPECT_TRUE(msg.getCategories().at(1) == "First");
    EXPECT_TRUE(msg.getCategories().at(2) == "Second");
    EXPECT_TRUE(msg.getCategories().at(3) == "Third");
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
    ASSERT_EQ(36+13, msg.getMessageSize());
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
    ASSERT_EQ(66, msg.getMessageSize());
    EXPECT_TRUE(msg.getCategoryName().compare("Simple Category") == 0);
    EXPECT_TRUE(msg.getUserName().compare("Bob") == 0);
    EXPECT_EQ(msg.getCategoryID(), -1);
    EXPECT_EQ(msg.getUserID(), -1);

    CategoryManagementMessage msg2(1, MessageType::JOIN_CATEGORY, 12, 44);
    ASSERT_EQ(MessageType::JOIN_CATEGORY, msg2.getMessageType());
    ASSERT_EQ(48, msg2.getMessageSize());
    ASSERT_EQ(0, msg2.getCategoryName().size());
    ASSERT_EQ(0, msg2.getUserName().size());
    EXPECT_EQ(msg2.getCategoryID(), 12);
    EXPECT_EQ(msg2.getUserID(), 44);
}

TEST(NeighboursInfoMessage, creating_test){
    NeighboursInfoMessage msg("Mathew", "10.9.5.3", 1234, "John", "11.3.5.1", 3456);

    ASSERT_EQ(MessageType::NEIGHBOURS_SET, msg.getMessageType());
    ASSERT_EQ(56+26, msg.getMessageSize());
    ASSERT_EQ(0, msg.getSenderID());
    EXPECT_TRUE( msg.getLeftNeighbourName().compare("Mathew") == 0);
    EXPECT_TRUE( msg.getRightNeighbourName().compare("John") == 0);
    EXPECT_TRUE( msg.getLeftNeighbourIP().compare("10.9.5.3") == 0);
    EXPECT_TRUE( msg.getRightNeighbourIP().compare("11.3.5.1") == 0);
    EXPECT_EQ( msg.getLeftNeighbourPort(), 1234);
    EXPECT_EQ( msg.getRightNeighbourPort(), 3456);
}

TEST(RingMessage, creating_test){
    RingMessage msg(1, "Message text");

    ASSERT_EQ(MessageType::RING_MESSAGE, msg.getMessageType());
    ASSERT_EQ(44, msg.getMessageSize());
    ASSERT_EQ(1, msg.getSenderID());
    EXPECT_TRUE(msg.getMsgText().compare("Message text") == 0);
    EXPECT_EQ(0, msg.getConfirmationsList().size());

    msg.addConfirmation("Ann");

    EXPECT_EQ(55, msg.getMessageSize());
    EXPECT_EQ(1, msg.getConfirmationsList().size());
    EXPECT_TRUE(msg.getConfirmationsList()[0] == "Ann");

    std::vector<std::string> names = {"Tom", "Kate"};
    msg.addConfirmations(names);

    EXPECT_EQ(78, msg.getMessageSize());
    EXPECT_EQ(3, msg.getConfirmationsList().size());
    EXPECT_TRUE(msg.getConfirmationsList()[0] == "Ann");
    EXPECT_TRUE(msg.getConfirmationsList()[1] == "Tom");
    EXPECT_TRUE(msg.getConfirmationsList()[2] == "Kate");

    RingMessage msg2 = msg;

    ASSERT_EQ(MessageType::RING_MESSAGE, msg2.getMessageType());
    ASSERT_EQ(78, msg2.getMessageSize());
    ASSERT_EQ(1, msg2.getSenderID());
    EXPECT_EQ(3, msg2.getConfirmationsList().size());
    EXPECT_TRUE(msg2.getConfirmationsList()[0] == "Ann");
    EXPECT_TRUE(msg2.getConfirmationsList()[1] == "Tom");
    EXPECT_TRUE(msg2.getConfirmationsList()[2] == "Kate");

}
