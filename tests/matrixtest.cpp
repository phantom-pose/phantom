#include "gtest/gtest.h"
#include "rotationmatrix.h"

TEST(matrix_test, simpleRotate)
{
    Point3D <float> point = { 0, 0, 0 };
    RotationMatrix matrix = { point, 0, M_PI / 2, M_PI / 2 }; // вращение вокруг оси х

    Point3D <float> p1 = { 0, 0, -1 };
    Point3D <float> p2 = { 0, 1, 0 };
    matrix.Rotate(p1);
    EXPECT_EQ(p1, p2);

    RotationMatrix matrix2 = { { 0, 1, 10 }, M_PI / 2, M_PI / 2, M_PI / 2 };

    matrix2.Rotate(p1);
    Point3D <float> p3 = { -10, 1, 10 };
    EXPECT_EQ(p3,p1);
}

TEST(matrix_test, complexRotate)
{
    RotationMatrix matrix = { { 0, 0, 0 }, 0, M_PI / 2, M_PI / 2 }; // вращение вокруг оси х

    Point3D <float> p1 = { 0, 0, -1 };
    Point3D <float> p2 = { 0, 1, 0 };
    matrix.Rotate(p1);
    EXPECT_EQ(p1, p2);
}

TEST(matrix_test, negaRotate)
{
    RotationMatrix matrix = { { 1, 2, 3 }, 0, M_PI / 3, M_PI / 6 };

    Point3D <float> p1 = { 42, 11, 74 };
    Point3D <float> p2 = p1;
    matrix.Rotate(p1);
    matrix.negaRotate(p1);
    EXPECT_EQ(p1, p2);
}
