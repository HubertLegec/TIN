//
// Created by hubert on 03.12.15.
//

#include "gtest/gtest.h"
#include "../client/model/headers/Model.h"
#include "../logger/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

TEST(ModelTest, model_server_info){
    Model model;
    model.setServerInfo("192.168.1.12",2137);
    ConnectionInfo serverInfo = model.getServerInfo();
    ASSERT_EQ(serverInfo.getIP(), "192.168.1.12");
    ASSERT_EQ(serverInfo.getPort(), 2137);
    ASSERT_EQ(serverInfo.getName(), "Server");
}

TEST(ModelTest, model_category_manipulations){
    Model model;
    model.addJoinedCategory(1,"jCategory");

    ASSERT_EQ(model.getCategoryId("jCategory"),1);
    ASSERT_EQ(model.getCategoryName(1),"jCategory");

    model.addMessageToCategory(1,"Lorem ipsum");
    ASSERT_EQ(model.getCategoryMessages(1).size(),1);

    ConnectionInfo neighbour("127.0.0.0",1111,"Damian");
    model.updateLeftNeighbour(1, neighbour);

    ASSERT_EQ(model.getLeftNeighbour(1).getName(),neighbour.getName());
    ASSERT_EQ(model.getLeftNeighbour(1).getPort(),neighbour.getPort());
    ASSERT_EQ(model.getLeftNeighbour(1).getIP(),neighbour.getIP());

    model.addMyCategory(2,"mCategory");
    ASSERT_EQ(model.getCategories().size(),2);
    ASSERT_EQ(model.getMyCategories().size(),1);
    ASSERT_EQ(model.getMyCategories()[2],"mCategory");
    ASSERT_EQ(0, model.getJoinedCategories().size());
    ASSERT_EQ(false, model.isCategoryActive(1));
    model.confirmCategory(1);
    ASSERT_EQ(1, model.getJoinedCategories().size());
    ASSERT_EQ(true, model.isCategoryActive(1));
    ASSERT_EQ(model.getJoinedCategories()[1],"jCategory");

    ASSERT_EQ(true, model.isCategoryActive(2));
    ASSERT_EQ(2, model.getActiveCategories().size());
    ASSERT_EQ(0, model.getInactiveCategories().size());
    ASSERT_EQ(model.getActiveCategories()[1],"jCategory");

    model.removeCategoryAndData(2);
    ASSERT_EQ(model.getCategories().size(),1);
}

TEST(ModelTest, user_data){
    Model model;

    ASSERT_FALSE(model.isRegistered());

    model.setUserName("Damian");
    model.setUserId(33);
    model.setMyIP("1.1.1.1");
    model.setMyPort(1111);

    ASSERT_TRUE(model.isRegistered());

    ASSERT_EQ(model.getUserName(),"Damian");
    ASSERT_EQ(model.getUserId(),33);
    ASSERT_EQ(model.getMyIP(),"1.1.1.1");
    ASSERT_EQ(model.getMyPort(),1111);
}


TEST(ModelTest, model_inbox){
    Model model;
    model.addMyCategory(2, "myCategory");
    ASSERT_EQ(0, model.getInboxMessages().size());
    RingMessage msg(1,2,"hello");
    model.addMessageToInbox(msg);
    ASSERT_EQ(1, model.getInboxMessages().size());
    model.markMessageAsRead(0);
    ASSERT_EQ(0, model.getInboxMessages().size());
    ASSERT_EQ(*model.getCategoryMessages(2).begin(),"hello");
}

TEST(ModelTest, model_notifications){
    Model model;

    model.addNotification("notification");
    ASSERT_TRUE(model.getNotifications().size()==1);
    ASSERT_EQ(*model.getNotifications().begin(),"notification");
    model.addNotification("notification2");
    model.clearNotificationList();
    ASSERT_TRUE(model.getNotifications().size()==0);
}
