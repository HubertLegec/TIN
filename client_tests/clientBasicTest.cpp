//
// Created by hubert on 03.12.15.
//

#include "gtest/gtest.h"
#include "../client/model/Model.h"

TEST(ModelTest, model_get_test){
    Model m;
    ASSERT_EQ(5, m.getX());
}


