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

void print(int length, const char *string) {
    for (int i = 0; i < length; ++i)
        std::cout << "Znak: " << i << " : " << ((int) string[i]) << std::endl;
}

const char *getcharFromString(std::string string) {
//    for (int i = 0; i < string.length(); ++i)
//        std::cout << "Test: " << i << ": " << (int) string[i] << std::endl;
    char *result = new char[string.length() + 1];
    for (int i = 0; i < string.length(); ++i) {
        result[i] = '\0';
    }
    int j = 0;
    for (int i = 0; i < string.length(); ++i) {
        if (((int) string[i]) != 0) {
            result[i] = string[i];
        }
    }
    result[string.length()] = -2;
    //print(string.length() + 1, result);
    return result;
}

std::string getStringFromChar(const char *tab) {
    int length = 0;
    while (true) {
//        std::cout << "Szukam: " << tab[length] << std::endl;
        if (((int) tab[length]) == -2) {
//            std::cout << "Dotarłem" << length;
            break;
        }
        length++;
    }
    if (length < 16)
        length = ((length / 16) + 1) * 16;
    std::string result;
    result.resize(length);
    for (int i = 0; i < length; ++i) {
        result[i] = tab[i];
//        if ((i % 4) == 0) {
////            std::cout << "Append" << i << std::endl;
//            result[(i / 4) * 4] = tab[i / 4];
//        }
//        else {
////            std::cout << "Append null" << i << std::endl;
//            result[i] = 0;
////            std::cout << "result length: " << result.length() << std::endl;
//        }
    }
    //for (int i = 0; i < result.length(); i++)
    //    std::cout << "Result: " << i << " : " << ((int) result[i]) << std::endl;
    return result;
}

TEST(SimpleMessage, serialization_test) {
    SimpleMessage msg(MessageType::SERVER_INFO, 1);

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
//        std::cout << "tu: " << ss.str() << std::endl << ss.str().c_str() << std::endl;
//        print(ss.str().length(), ss.str().c_str());
    }
    const char *constChar = getcharFromString(ss.str());

    std::stringstream sso(getStringFromChar(constChar));
    SimpleMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(sso); // Create an input archive
        iarchive(testMsg);
//        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
//        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
}


TEST(ServerInfoMessage, serialization_test) {
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

TEST(GetMessage, serialization_test) {
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

TEST(CategoryListMessage, serialization_test) {
    std::map<long, std::string> cat = {{1, "First"},
                                       {2, "Second"},
                                       {3, "Third"}};
    CategoryListMessage msg(0, cat);

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
    }

    CategoryListMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
    for (int i = 1; i <= 3; i++) {
        EXPECT_TRUE(msg.getCategories().at(i).compare(testMsg.getCategories().at(i)) == 0);
    }
}

TEST(CategoryManagementMessage, serialization_test) {
    CategoryManagementMessage msg(1, MessageType::CREATE_CATEGORY, "Simple Category");
    msg.setExtraInfo(444);

    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        oarchive(msg); // Write the data to the archive
    }
    const char *constChar = getcharFromString(ss.str());

    std::stringstream sso(getStringFromChar(constChar));
    CategoryManagementMessage testMsg;
    {
        cereal::BinaryInputArchive iarchive(sso); // Create an input archive
        iarchive(testMsg); // Read the data from the archive
    }

    ASSERT_EQ(msg.getMessageSize(), testMsg.getMessageSize());
    ASSERT_EQ(msg.getMessageType(), testMsg.getMessageType());
    ASSERT_EQ(msg.getSenderID(), testMsg.getSenderID());
    EXPECT_EQ(msg.getCategoryID(), testMsg.getCategoryID());
    EXPECT_TRUE(msg.getCategoryName() == testMsg.getCategoryName());
    EXPECT_EQ(msg.getExtraInfo(), testMsg.getExtraInfo());

    std::stringstream ss2;

    CategoryManagementMessage msg2(1, MessageType::DESTROY_CATEGORY, 12);

    {
        cereal::BinaryOutputArchive oarchive(ss2); // Create an output archive
        oarchive(msg2); // Write the data to the archive
    }

    CategoryManagementMessage testMsg2;
    {
        cereal::BinaryInputArchive iarchive(ss2); // Create an input archive
        iarchive(testMsg2); // Read the data from the archive
    }

    ASSERT_EQ(msg2.getMessageSize(), testMsg2.getMessageSize());
    ASSERT_EQ(msg2.getMessageType(), testMsg2.getMessageType());
    ASSERT_EQ(msg2.getSenderID(), testMsg2.getSenderID());
    EXPECT_EQ(msg2.getCategoryID(), testMsg2.getCategoryID());
    EXPECT_TRUE(msg2.getCategoryName() == testMsg2.getCategoryName());
}

TEST(RingMessage, serialization_test) {
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