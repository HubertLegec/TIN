#include "gtest/gtest.h"
#include "../server/model/headers/Model.h"
#include "../server/utils/ServerGlobalConstants.h"

TEST(modelTest, modelTest_user_management) {
    Model model;

    // id 1
    model.createNewUser("radek", 1111, "1.1.1.1");
    // id 2
    model.createNewUser("damian", 2222, "2.2.2.2");
    // id 3
    model.createNewUser("wojtek", 3333, "3.3.3.3");
    // id 4
    model.createNewUser("hubert", 4444, "4.4.4.4");

    auto user_1 = model.getUser(1);
    ASSERT_EQ(user_1->getID(), 1);
    ASSERT_EQ(user_1->getName(), "radek");
    ASSERT_EQ(user_1->getIP(), "1.1.1.1");
    ASSERT_EQ(user_1->getPort(), 1111);

    auto user_2 = model.getUser(2);
    ASSERT_EQ(user_2->getID(), 2);
    ASSERT_EQ(user_2->getName(), "damian");
    ASSERT_EQ(user_2->getIP(), "2.2.2.2");
    ASSERT_EQ(user_2->getPort(), 2222);

    auto user_3 = model.getUser(3);
    ASSERT_EQ(user_3->getID(), 3);
    ASSERT_EQ(user_3->getName(), "wojtek");
    ASSERT_EQ(user_3->getIP(), "3.3.3.3");
    ASSERT_EQ(user_3->getPort(), 3333);

    auto user_4 = model.getUser(4);
    ASSERT_EQ(user_4->getID(), 4);
    ASSERT_EQ(user_4->getName(), "hubert");
    ASSERT_EQ(user_4->getIP(), "4.4.4.4");
    ASSERT_EQ(user_4->getPort(), 4444);

    ASSERT_FALSE(model.getUser(0));
    ASSERT_FALSE(model.getUser(5));

    model.deleteUser(user_3);
    ASSERT_FALSE(model.getUser(3));
}

TEST(modelTest, modelTest_category_management) {
    Model model;

    // id 1
    model.createNewUser("radek", 1111, "1.1.1.1");
    // id 2
    model.createNewUser("damian", 2222, "2.2.2.2");
    // id 3
    model.createNewUser("wojtek", 3333, "3.3.3.3");
    // id 4
    model.createNewUser("hubert", 4444, "4.4.4.4");
    auto user_1 = model.getUser(1);
    auto user_2 = model.getUser(2);
    auto user_3 = model.getUser(3);
    auto user_4 = model.getUser(4);

    ASSERT_EQ(model.createCategory(user_1, "category_0"), 0);
    ASSERT_EQ(model.createCategory(user_2, "category_1"), 1);
    ASSERT_EQ(model.createCategory(user_3, "category_2"), 2);
    ASSERT_EQ(model.createCategory(user_4, "category_3"), 3);

    ASSERT_EQ(model.createCategory(user_1, "category_0"), ServerGlobalConstant::FAILED_CODE);
    ASSERT_EQ(model.createCategory(user_2, "category_1"), ServerGlobalConstant::FAILED_CODE);
    ASSERT_EQ(model.createCategory(user_3, "category_2"), ServerGlobalConstant::FAILED_CODE);
    ASSERT_EQ(model.createCategory(user_4, "category_3"), ServerGlobalConstant::FAILED_CODE);

    auto category_1 = model.getCategory(1);
    ASSERT_EQ(category_1->getName(), "category_1");
    ASSERT_EQ(category_1->getID(), 1);
    ASSERT_EQ(category_1->getOwner(), user_2);

    ASSERT_EQ(model.getCategory(0)->getName(), "category_0");
    ASSERT_EQ(model.getCategory(1)->getName(), "category_1");
    ASSERT_EQ(model.getCategory(2)->getName(), "category_2");
    ASSERT_EQ(model.getCategory(3)->getName(), "category_3");
    ASSERT_FALSE(model.getCategory(4));

    model.deleteCategory(category_1);
    ASSERT_FALSE(model.getCategory(1));
    ASSERT_TRUE(model.getCategory(0)->getName() == "category_0");
    ASSERT_TRUE(model.getCategory(2)->getName() == "category_2");
    ASSERT_TRUE(model.getCategory(3)->getName() == "category_3");
}