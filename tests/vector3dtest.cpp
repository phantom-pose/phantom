#include "gtest/gtest.h"
#include "vector3d.h"

TEST(vector3d_test, construction)
{
    Vector3D vec1;
    Point3D <float> p1, p2 = { 0, 0, 1 };
    Point3D <float> _p1 = vec1.getPosition();
    Point3D <float> _p2 = vec1.getDirection();
    EXPECT_EQ(p1,_p1);
    EXPECT_EQ(p2,_p2);
}
