#include "gtest/gtest.h"
#include "boxnet.h"

TEST(boxNet_test, test_construction)
{
    BoxNet bN = { 5, 10, 15 };
    EXPECT_EQ(bN.getSizeX(), 5);
    EXPECT_EQ(bN.getSizeY(), 10);
    EXPECT_EQ(bN.getSizeZ(), 15);
}
