#include "gtest/gtest.h"
#include "point3d.hpp"
#include "logger.h"

TEST(point3D_test, construction)
{
    // Конструктор по умолчанию для типов int и float
    Point3D <int> point1;
    EXPECT_EQ( point1.x(), 0 );
    EXPECT_EQ( point1.y(), 0 );
    EXPECT_EQ( point1.z(), 0 );
    Point3D <float> point2;
    EXPECT_EQ( point2.x(), 0.0f );
    EXPECT_EQ( point2.y(), 0.0f );
    EXPECT_EQ( point2.z(), 0.0f );
    // Конструктор с параметрами для типов int и float
    Point3D <int> point3 = { 42, 23, 815 };
    EXPECT_EQ( point3.x(), 42 );
    EXPECT_EQ( point3.y(), 23 );
    EXPECT_EQ( point3.z(), 815 );
    Point3D <float> point4 = { 42.4, 23.16, 815.2342 };
    EXPECT_LT( point4.x(), 42.4001 );
    EXPECT_GT( point4.x(), 42.3999 );
    EXPECT_LT( point4.y(), 23.160001 );
    EXPECT_GT( point4.y(), 23.159999 );
    EXPECT_LT( point4.z(), 815.23421 );
    EXPECT_GT( point4.z(), 815.23419 );
    // Конструктор перемещения
    Point3D <int> point5 = { 42, 23, 815 };
    Point3D <int> point6( std::move(point5) );
    EXPECT_EQ( point6, point3 );
    EXPECT_EQ( point5, point1 );
    // Конструктор копирования
    Point3D <int> point7 = { 42, 23, 815 };
    Point3D <int> point8(point7);
    EXPECT_EQ( point7, point8 );
}

TEST(point3D_test, equals)
{
    Point3D <int> point1 = { 0, 0, 0 };
    Point3D <int> point2;

    EXPECT_TRUE( point1 == point2 );

    Point3D <float> point3 = { 1.2, 2.3, 3.4 };
    Point3D <float> point4 = { 1.2, 2.3, 3.4 };
    Point3D <float> point5 = { 1.2001, 2.3, 3.4 };

    EXPECT_TRUE( point3 == point4 );
    EXPECT_EQ( point3, point4 );
    EXPECT_NE( point3, point5 );
}

TEST(point3D_test, copy_and_move)
{
    Point3D <int> point1 = { 42, 23, 815 };
    Point3D <int> point2;
    point2 = point1;
    EXPECT_EQ( point1, point2 );

    Point3D <int> point3 = { 42, 23, 815 };
    Point3D <int> point4;
    Point3D <int> point5;
    Point3D <int> point6 = { 42, 23, 815 };

    point4 = std::move(point3);
    EXPECT_EQ( point4, point6 );
    EXPECT_EQ( point3, point5 );
}

TEST(point3D_test, sum_operator)
{
    Point3D <float> point1 = { 1.23, 4.56, 7.89 };
    Point3D <float> point2 = { 4.81, 5.16, 2.342 };

    Point3D <float> point3 = point1 + point2;
    Point3D <float> point4 = { 6.04, 9.72, 10.232 };
    EXPECT_EQ( point3, point4 );
}

TEST(point3D_test, subtraction_operator)
{
    Point3D <float> point1 = { 16.23, 9.56, 7.89 };
    Point3D <float> point2 = { 4.81, 5.16, 2.342 };

    Point3D <float> point3 = point1 - point2;
    Point3D <float> point4 = { 11.42, 4.4, 5.548 };
    EXPECT_EQ( point3, point4 );
}

TEST(point3D_test, scale_operator)
{
    Point3D <float> point1 = { 16.23, 9.56, 7.89 };
    float scale = 1.2;
    Point3D <float> point3 = point1 * scale;
    Point3D <float> point4 = { 19.476, 11.472, 9.468 };
    EXPECT_EQ( point3, point4 );
}
