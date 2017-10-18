#include "gtest/gtest.h"
#include "boxnet.h"
#include "slice.h"
#include "point3d.hpp"

TEST(boxNet_test, test_sumple_constructor)
{
    BoxNet bN = { 5, 10, 15 };
    EXPECT_EQ(bN.getSizeX(), 5);
    EXPECT_EQ(bN.getSizeY(), 10);
    EXPECT_EQ(bN.getSizeZ(), 15);
}

TEST(boxNet_test, test_copy_constructor)
{
    BoxNet box1 = { 3, 4, 5 };
    for (int i = 0; i < 3 * 4 * 5; i++) {
        box1.setByNum(i, i);
    }
    BoxNet box2 = { box1 };
    EXPECT_EQ(box1, box2);
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

TEST(boxNet_test, position)
{
    BoxNet box = { 3, 4, 5 };
    Point3D <int> point1 = box.position();
    Point3D <int> point2;
    EXPECT_EQ( point1, point2 );
}

TEST(boxNet_test, grow)
{
    BoxNet box = { 3, 4, 5 };
    for (int i = 0; i < 3 * 4 * 5; i++) {
        box.setByNum(i, i);
    }

    Point3D <int> sizes = { 5, 7, 9 };
    Point3D <int> position = { 0, 0, 0 };

    box.grow(sizes, position);
}

TEST(boxNet_test, getXYZ)
{
    BoxNet box = { 3, 4, 5 };
    for (int i = 0; i < 3 * 4 * 5; i++) {
        box.setByNum(i, i);
    }

    Point3D <int> xyz0 = box.getXYZ(0);
    Point3D <int> _xyz0 = { 0, 0, 0 };
    Point3D <int> xyz1 = box.getXYZ(1);
    Point3D <int> _xyz1 = { 1, 0, 0 };
    Point3D <int> xyz17 = box.getXYZ(17);
    Point3D <int> _xyz17 = { 2, 1, 1 };
    Point3D <int> xyz45 = box.getXYZ(45);
    Point3D <int> _xyz45 = { 0, 3, 3 };
    Point3D <int> xyz59 = box.getXYZ(59);
    Point3D <int> _xyz59 = { 2, 3, 4 };

    EXPECT_EQ(xyz0, _xyz0);
    EXPECT_EQ(xyz1, _xyz1);
    EXPECT_EQ(xyz17, _xyz17);
    EXPECT_EQ(xyz45, _xyz45);
    EXPECT_EQ(xyz59, _xyz59);
}
