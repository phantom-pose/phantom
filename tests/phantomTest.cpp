#include "gtest/gtest.h"
#include "phantom.h"
#include "boxnet.h"
#include "netpaintarea.h"
#include "slice.h"
#include "logger.h"

TEST(phantom_test, test_memory)
{
    BoxNet box = { 3, 4, 5 };
    for (int i = 0; i < 3 * 4 * 5; i++) {
        box.setByNum(i, i);
    }
    Phantom * phantom;
    phantom = new Phantom(box);
    Phantom phantom2 = { box };
    BoxNet const * box2;

    box2 = &phantom->boxNet();

    BoxNet const box1 = phantom2.boxNet();

//    Phantom phantom2;
//    BoxNet box2;

//    box2 = phantom2.boxNet();
    Slice slice = phantom2.getSliceZ(1);

//    Slice slice = box1.getSliceX(1);
//    for (int y = 0; y < slice.getSizeY(); y++) {
//        for (int x = 0; x < slice.getSizeX(); x++) {
//            EXPECT_EQ(slice.getValue(x, y), 12 + y * 3 + x);
//        }
//    }
    delete phantom;
}

TEST(phantom_test, test_constructor)
{
    // create box
    BoxNet box = { 3, 4, 5 };
    for (int i = 0; i < 3 * 4 * 5; i++) {
        box.setByNum(i, i);
    }
    Phantom phantom = { box };
    BoxNet box2 = phantom.boxNet();
    EXPECT_EQ(box2, box);

    Phantom * phantom2;
    phantom2 = new Phantom( box );

    BoxNet const * box3;
    BoxNet const * box4;
    box4 = &box;
    box3 = &phantom2->boxNet();
    EXPECT_EQ(*box3, box);
    delete phantom2;
}

TEST(phantom_test, center)
{
    BoxNet box = { 3, 4, 5 };
    for (int i = 0; i < 3 * 4 * 5; i++) {
        box.setByNum(i, i);
    }
    Phantom phantom = { box };

    Point3D <float> p0 = phantom.center(0);
    Point3D <float> _p0 = { 0.8875, 0.8875, 1.21};

    Point3D <float> p1 = phantom.center(1);
    Point3D <float> _p1 = { 2.6625, 0.8875, 1.21};

    Point3D <float> p17 = phantom.center(17);
    Point3D <float> _p17 = { 4.4375, 2.6625, 3.63};

    Point3D <float> p59 = phantom.center(59);
    Point3D <float> _p59 = { 4.4375, 6.2125, 10.89};

    EXPECT_EQ(p0, _p0);
    EXPECT_EQ(p1, _p1);
    EXPECT_EQ(p17, _p17);
    EXPECT_EQ(p59, _p59);
}
