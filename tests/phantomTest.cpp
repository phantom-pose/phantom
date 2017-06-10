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
