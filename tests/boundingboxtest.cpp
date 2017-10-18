#include "gtest/gtest.h"
#include "boundingbox.h"
#include "rotationmatrix.h"

TEST(boundingBox_test, test_rotate)
{
    BoundingBox bb = { 0, 0, 0, 10, 20, 30 };
    RotationMatrix matrix( { 0, 0, 50 }, 0, M_PI / 2, M_PI / 2);
    bb.rotate(matrix);
    Point3D <float> pos = { 0, 50, 50 };
    Point3D <float> ex  = { 1, 0, 0 };
    Point3D <float> ey  = { 0, 0, 1 };
    Point3D <float> ez  = { 0, -1, 0 };
    EXPECT_EQ(bb.getPosition(), pos);
    EXPECT_EQ(bb.getEx(), ex);
    EXPECT_EQ(bb.getEy(), ey);
    EXPECT_EQ(bb.getEz(), ez);
}

TEST(boundingBox_test, test_intersect_parallel_x)
{
    // Использую вращение из предыдущего пункта
    BoundingBox bb = { 0, 0, 0, 10, 20, 30 };
    RotationMatrix matrix( { 0, 0, 50 }, 0, M_PI / 2, M_PI / 2);
    bb.rotate(matrix);
    Line line(-2, 30, 60, 1, 0, 0);
    float tmin = 10000, tmax = -10000;
    int err = bb.intersect(line, tmin, tmax);
    std::cout << "err = " << err << "\n";
    EXPECT_EQ(err, 0);
    if (!err) {
        EXPECT_EQ(tmin, 2);
        EXPECT_EQ(tmax, 12);
    }
}
