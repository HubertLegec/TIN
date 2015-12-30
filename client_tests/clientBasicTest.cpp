//
// Created by hubert on 03.12.15.
//

#include "gtest/gtest.h"
#include "../client/model/headers/Model.h"

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
    model.updateRightNeighbour(1, neighbour);

    ASSERT_EQ(model.getLeftNeighbour(1).getName(),neighbour.getName());
    ASSERT_EQ(model.getLeftNeighbour(1).getPort(),neighbour.getPort());
    ASSERT_EQ(model.getLeftNeighbour(1).getIP(),neighbour.getIP());
    ASSERT_EQ(model.getRightNeighbour(1).getName(),neighbour.getName());
    ASSERT_EQ(model.getRightNeighbour(1).getPort(),neighbour.getPort());
    ASSERT_EQ(model.getRightNeighbour(1).getIP(),neighbour.getIP());

    model.addMyCategory(2,"mCategory");
    ASSERT_EQ(model.getCategories().size(),2);
    ASSERT_EQ(model.getMyCategories().size(),1);
    ASSERT_EQ(model.getMyCategories()[2],"mCategory");
    ASSERT_EQ(model.getJoinedCategories().size(),1);
    ASSERT_EQ(model.getJoinedCategories()[1],"jCategory");

    model.setCategoryActive(1,true);
    ASSERT_EQ(model.isCategoryActive(1),true);
    ASSERT_EQ(model.isCategoryActive(2),false);
    ASSERT_EQ(model.getActiveCategories().size(),1);
    ASSERT_EQ(model.getInactiveCategories().size(),1);
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

    ASSERT_EQ(model.getInboxMessages().size(),0);
    RingMessage msg(1,2,"hello");
    model.addMessageToInbox(msg);
    ASSERT_EQ(model.getInboxMessages().size(),1);
    model.markMessageAsRead(0);
    ASSERT_EQ(model.getInboxMessages().size(),0);
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
