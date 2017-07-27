#include "gtest/gtest.h"
#include "bodypart.h"
#include <vector>
#include "logger.h"

TEST(boxNet_test, fill)
{
    BodyPart leftHand2;
    leftHand2.fillData("data/bodyparts/leftHand-2.bin");
    int n = 0;

//    BodyPart const & bp = leftHand2;
//    bp.setRotPoint( { 1.12, 2.3, 3.66 } );
//    Point3D <float> p2 = leftHand2.getRotPoint();
//    Logger::Instance() << p2;

//    leftHand2.setRotPoint( { 1.12, 2.3, 3.66 } );
//    Point3D <float> p = leftHand2.getRotPoint();
//    Logger::Instance() << p;

//    for (auto it = leftHand2.data.begin(); it != leftHand2.data.end(); it++) {
//        n++;
//    }

//    Logger::Instance() << n << "\n";

//    data1.size()

}
