#include "gtest/gtest.h"
#include "boxnet.h"

TEST(boxNet_test, test_construction)
{
    BoxNet bN = { 5, 10, 15 };
    EXPECT_EQ(bN.getSizeX(), 5);
    EXPECT_EQ(bN.getSizeY(), 10);
    EXPECT_EQ(bN.getSizeZ(), 15);
}

TEST(boxNet_test, test_fillFromFile)
{
//    BoxNet bN = { 299, 137, 348 };
//    bN.fillFromFile("data/AF.dat");
//    bN.writeBinFile("data/AF_bin.dat");
//    BoxNet bN2 = { 299, 137, 348 };
//    bN2.fillFromBin("data/AF_bin.dat");
//    for (int i = 0; i < bN.getSizeX(); i++) {
//        for (int j = 0; j < bN.getSizeY(); j++) {
//            for (int k = 0; k < bN.getSizeZ(); k++) {
//                EXPECT_EQ(bN.getByXyz(i,j,k), bN2.getByXyz(i,j,k));
//            }
//        }
//    }
}
