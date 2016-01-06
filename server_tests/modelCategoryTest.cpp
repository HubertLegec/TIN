#include "gtest/gtest.h"
#include "../server/model/headers/Category.h"

TEST(modelCategoryTest, categoryTest_creating_category) {
    shared_ptr<User> creator(new User(1, 9999, "1.1.1.1", "radek"));
    Category category(0, creator, "test_category");

    ASSERT_EQ(category.getID(), 0);
    ASSERT_EQ(category.getName(), "test_category");
    ASSERT_EQ(category.getOwner(), creator);
}

TEST(modelCategoryTest, categoryTest_members_management) {
    shared_ptr<User> creator(new User(1, 9999, "1.1.1.1", "radek"));
    shared_ptr<User> user_1(new User(2, 8888, "1.1.1.1", "hubert"));
    shared_ptr<User> user_2(new User(3, 7777, "1.1.1.1", "damian"));
    shared_ptr<User> user_3(new User(4, 6666, "1.1.1.1", "wojtek"));
    Category category(0, creator, "test_category");

    category.addMember(user_1);
    category.addMember(user_2);
    category.addMember(user_3);

    ASSERT_EQ(category.findMember(1)->getUser(), creator);
    ASSERT_EQ(category.findMember(2)->getUser(), user_1);
    ASSERT_EQ(category.findMember(3)->getUser(), user_2);
    ASSERT_EQ(category.findMember(4)->getUser(), user_3);

    category.removeMember(2);
    ASSERT_FALSE(category.findMember(2));
    ASSERT_EQ(category.findMember(1)->getUser(), creator);
    ASSERT_EQ(category.findMember(3)->getUser(), user_2);
    ASSERT_EQ(category.findMember(4)->getUser(), user_3);

    category.removeMember(1);
    ASSERT_FALSE(category.findMember(1));
    ASSERT_FALSE(category.findMember(2));
    ASSERT_EQ(category.findMember(3)->getUser(), user_2);
    ASSERT_EQ(category.findMember(4)->getUser(), user_3);

    category.removeMember(3);
    ASSERT_FALSE(category.findMember(1));
    ASSERT_FALSE(category.findMember(2));
    ASSERT_FALSE(category.findMember(3));
    ASSERT_EQ(category.findMember(4)->getUser(), user_3);

    category.removeMember(4);
    ASSERT_FALSE(category.findMember(1));
    ASSERT_FALSE(category.findMember(2));
    ASSERT_FALSE(category.findMember(3));
    ASSERT_FALSE(category.findMember(4));

    category.addNewMember(user_1);
    category.addNewMember(user_2);
    category.addNewMember(user_3);

    ASSERT_TRUE(category.isUnconfirmed(2));
    ASSERT_TRUE(category.isUnconfirmed(3));
    ASSERT_TRUE(category.isUnconfirmed(4));

    category.acceptNewUser(2);
    ASSERT_FALSE(category.isUnconfirmed(2));
    ASSERT_EQ(category.findMember(2)->getUser(), user_1);

    category.acceptNewUser(3);
    ASSERT_FALSE(category.isUnconfirmed(3));
    ASSERT_EQ(category.findMember(2)->getUser(), user_1);
    ASSERT_EQ(category.findMember(3)->getUser(), user_2);

    category.rejectMember(4);
    ASSERT_FALSE(category.isUnconfirmed(4));
    ASSERT_FALSE(category.findMember(4));
}