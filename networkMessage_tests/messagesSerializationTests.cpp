//
// Created by hubert on 12.12.15.
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

TEST(SimpleMessage, serialization_test){
    SimpleMessage msg(MessageType::SERVER_INFO, 1);

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
    }

    SimpleMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
}

TEST(ServerInfoMessage, serialization_test){
    ServerInfoMessage msg(13, ServerInfoMessageType::OK, "info");

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
    }

    ServerInfoMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
    EXPECT_EQ(msg.getInfoType(), testMsg.getInfoType());
    EXPECT_TRUE(msg.getInfo() == testMsg.getInfo());
}

TEST(NeighboursInfoMessage, serialization_test) {
    NeighboursInfoMessage msg(3, "Mathew", "10.9.5.3", 1234, "John", "11.3.5.1", 3456);

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
    }

    std::ofstream file;
    file.open("/home/hubert/neighserial");
    file << ss.str();
    file.close();

    NeighboursInfoMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
    EXPECT_EQ(msg.getCategoryId(), testMsg.getCategoryId());
    EXPECT_EQ(msg.getLeftNeighbourPort(), testMsg.getLeftNeighbourPort());
    EXPECT_EQ(msg.getRightNeighbourPort(), testMsg.getRightNeighbourPort());
    EXPECT_TRUE(msg.getLeftNeighbourName() == testMsg.getLeftNeighbourName());
    EXPECT_TRUE(msg.getLeftNeighbourIP() == testMsg.getLeftNeighbourIP());
    EXPECT_TRUE(msg.getRightNeighbourName() == testMsg.getRightNeighbourName());
    EXPECT_TRUE(msg.getRightNeighbourIP() == testMsg.getRightNeighbourIP());
}

TEST(GetMessage, serialization_test){
    GetMessage msg(0, GetMessageType::CAT_LIST);

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
    }

    GetMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
    EXPECT_EQ(msg.getRequestType(), testMsg.getRequestType());
}

TEST(CategoryListMessage, serialization_test){
    std::map<long, std::string> cat = {{1, "First"}, {2, "Second"}, {3, "Third"}};
    CategoryListMessage msg(0, cat);

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
    }

/*std::ofstream file;
file.open("/home/hubert/catListSer");
file << ss.str();
file.close();*/

    CategoryListMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
    for(int i = 1; i <= 3; i++){
        EXPECT_TRUE( msg.getCategories().at(i).compare(testMsg.getCategories().at(i)) == 0);
    }
}

TEST(CategoryManagementMessage, serialization_test){
    CategoryManagementMessage msg(1, MessageType::CREATE_CATEGORY, "Simple Category", 333);

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
    }

    CategoryManagementMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
    EXPECT_EQ(msg.getCategoryID(), testMsg.getCategoryID());
    EXPECT_EQ(msg.getUserID(), testMsg.getUserID());
    EXPECT_TRUE(msg.getCategoryName() == testMsg.getCategoryName());

    CategoryManagementMessage msg2(1, MessageType::DESTROY_CATEGORY, 12, 13);

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg2); // Write the data to the archive
    }

    CategoryManagementMessage testMsg2;
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(testMsg2); // Read the data from the archive
    }

    ASSERT_EQ(msg2.getMessageSize(), testMsg2.getMessageSize());
    ASSERT_EQ(msg2.getMessageType(), testMsg2.getMessageType());
    ASSERT_EQ(msg2.getSenderID(), testMsg2.getSenderID());
    EXPECT_EQ(msg2.getCategoryID(), testMsg2.getCategoryID());
    EXPECT_EQ(msg2.getUserID(), testMsg2.getUserID());
    EXPECT_TRUE(msg2.getCategoryName() == testMsg2.getCategoryName());
}

TEST(RingMessage, serialization_test){
    RingMessage msg(1, 5, "Message text");
    msg.addConfirmation("Ann");

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
    }

    RingMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
    EXPECT_TRUE(msg.getMsgText() == testMsg.getMsgText());
    EXPECT_EQ(msg.getCategoryId(), testMsg.getCategoryId());
    EXPECT_EQ(msg.getConfirmationsList().size(), testMsg.getConfirmationsList().size());
    EXPECT_TRUE(msg.getConfirmationsList()[0] == testMsg.getConfirmationsList()[0]);
}