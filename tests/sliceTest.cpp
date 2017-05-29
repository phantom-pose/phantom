#include "gtest/gtest.h"
#include "slice.h"

TEST(slice_test, test_construction)
{
    Slice slice1 = { 3, 4 };
    EXPECT_EQ(slice1.getPropX(), 1);
    EXPECT_EQ(slice1.getPropY(), 1);
    // Check proportion when x < y
    Slice slice2 = { 13, 42, 3, 8 };
    EXPECT_EQ(slice2.getPropX(), 1);
    EXPECT_LT(slice2.getPropY(), 2.667);
    EXPECT_GT(slice2.getPropY(), 2.666);
    // Check proportion when x > y
    Slice slice3 = { 13, 42, 8, 3 };
    EXPECT_EQ(slice3.getPropY(), 1);
    EXPECT_LT(slice3.getPropX(), 2.667);
    EXPECT_GT(slice3.getPropX(), 2.666);
}

//TEST(slice_test, test_filling)
//{
//    Slice slice4 = { 3, 4 };
//    for (int i = 0; i < slice4.getSizeX(); i++) {
//        for (int j = 0; j < slice4.getSizeY(); j++) {
//            slice4.setValue(i, j, i * j);
//        }
//    }
//    for (int i = 0; i < slice4.getSizeX(); i++) {
//        for (int j = 0; j < slice4.getSizeY(); j++) {
//            EXPECT_EQ(slice4.getValue(i,j), i * j);
//        }
//    }
//}

TEST(slice_test, test_move_constructor)
{
    Slice slice1 = { 3, 4 };
    for (int i = 0; i < slice1.getSizeX(); i++) {
        for (int j = 0; j < slice1.getSizeY(); j++) {
            slice1.setValue(i, j, i * j);
        }
    }

    Slice slice2 = { std::move(slice1) };
    for (int i = 0; i < slice2.getSizeX(); i++) {
        for (int j = 0; j < slice2.getSizeY(); j++) {
            EXPECT_EQ(slice2.getValue(i,j), i * j);
        }
    }
}
