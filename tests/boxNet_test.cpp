#include "gtest/gtest.h"
#include "boxnet.h"
#include "slice.h"

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

TEST(boxNet_test, test_slice_return)
{
    BoxNet box = { 3, 4, 5 };
    for (int i = 0; i < 3 * 4 * 5; i++) {
        box.setByNum(i, i);
    }

    Slice slice = box.getSliceZ(1);
    for (int y = 0; y < slice.getSizeY(); y++) {
        for (int x = 0; x < slice.getSizeX(); x++) {
            EXPECT_EQ(slice.getValue(x, y), 12 + y * 3 + x);
        }
    }
}

TEST(boxNet_test, test_segmentation)
{
    BoxNet box = { 3, 4, 5 };
    for (int i = 0; i < 3 * 4 * 5; i++) {
        box.setByNum(i, i);
    }
    EXPECT_EQ(box.getSizeZ(), 5);

    box.segmentation();

    EXPECT_EQ(box.getSizeZ(), 10);

    Slice slice0 = box.getSliceZ(0);
    for (int y = 0; y < slice0.getSizeY(); y++) {
        for (int x = 0; x < slice0.getSizeX(); x++) {
            EXPECT_EQ(slice0.getValue(x, y), 0 + y * 3 + x);
        }
    }
    Slice slice1 = box.getSliceZ(1);
    for (int y = 0; y < slice1.getSizeY(); y++) {
        for (int x = 0; x < slice1.getSizeX(); x++) {
            EXPECT_EQ(slice1.getValue(x, y), 0 + y * 3 + x);
        }
    }
    Slice slice2 = box.getSliceZ(2);
    for (int y = 0; y < slice2.getSizeY(); y++) {
        for (int x = 0; x < slice2.getSizeX(); x++) {
            EXPECT_EQ(slice2.getValue(x, y), 12 + y * 3 + x);
        }
    }
}
