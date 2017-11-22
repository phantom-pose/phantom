#include "gtest/gtest.h"
#include "vector3d.h"

TEST(vector3d_test, construction)
{
    // default constructor
    Vector3D vec1;
    Point3D <float> p1, p2 = { 0, 0, 1 };
    Point3D <float> _p1 = vec1.getPosition();
    Point3D <float> _p2 = vec1.getDirection();
    EXPECT_EQ(p1,_p1);
    EXPECT_EQ(p2,_p2);

    // direction-float constructor
    Vector3D vec2 = { 1, 2, 3 };
    Point3D <float> dir = vec2.getDirection();
    Point3D <float> pos = vec2.getPosition();
    Point3D <float> ep = vec2.getEndpoint();
    float len = vec2.getLength();

    Point3D <float> _dir = { 0.267261, 0.534522, 0.801784 };
    Point3D <float> _pos, _ep = _dir;
    float _len = 1;
    EXPECT_EQ(dir, _dir);
    EXPECT_EQ(pos, _pos);
    EXPECT_EQ(ep, _ep);
    EXPECT_EQ(len, _len);

    // direction-point constructor
    Vector3D vec3 = { { 1, 2, 3 } };
    Point3D <float> dir3 = vec3.getDirection();
    Point3D <float> pos3 = vec3.getPosition();
    Point3D <float> ep3 = vec3.getEndpoint();
    float len3 = vec3.getLength();

    Point3D <float> _dir3 = { 0.267261, 0.534522, 0.801784 };
    Point3D <float> _pos3, _ep3 = _dir3;
    float _len3 = 1;
    EXPECT_EQ(dir3, _dir3);
    EXPECT_EQ(pos3, _pos3);
    EXPECT_EQ(ep3, _ep3);
    EXPECT_EQ(len3, _len3);
}

TEST(vector3d_test, const_pos_dir)
{
    // pos-dir constructor
    Vector3D vec1 = { { 1, 2, 3 }, { 3, 2, 1 } };
    Point3D <float> dir1 = vec1.getDirection();
    Point3D <float> pos1 = vec1.getPosition();
    Point3D <float> ep1 = vec1.getEndpoint();
    float len1 = vec1.getLength();

    Point3D <float> _pos1 = { 1, 2, 3 };
    Point3D <float> _dir1 = { 0.801784, 0.534522, 0.267261 };
    Point3D <float> _ep1 = _dir1 + _pos1;
    float _len1 = 1;

    EXPECT_EQ(dir1, _dir1);
    EXPECT_EQ(pos1, _pos1);
    EXPECT_EQ(ep1, _ep1);
    EXPECT_EQ(len1, _len1);

    // pos-dir-len constructor
    Vector3D vec2 = { { 1, 2, 3 }, { 3, 2, 1 }, 15 };
    Point3D <float> dir2 = vec2.getDirection();
    Point3D <float> pos2 = vec2.getPosition();
    Point3D <float> ep2 = vec2.getEndpoint();
    float len2 = vec2.getLength();

    Point3D <float> _pos2 = { 1, 2, 3 };
    Point3D <float> _dir2 = { 0.801784, 0.534522, 0.267261 };
    float _len2 = 15;
    Point3D <float> _ep2 = _dir2 * _len2 + _pos2;

    EXPECT_EQ(dir2, _dir2);
    EXPECT_EQ(pos2, _pos2);
    EXPECT_EQ(ep2, _ep2);
    EXPECT_EQ(len2, _len2);
}

TEST(vector3d_test, set_endpoint)
{
    Vector3D vec;
    vec.setEndpoint( { -5, -5, -5 } );
    Point3D <float> dir = vec.getDirection();
    Point3D <float> pos = vec.getPosition();
    Point3D <float> ep = vec.getEndpoint();

    Point3D <float> _dir = { -0.57735, -0.57735, -0.57735 };
    Point3D <float> _pos;
    Point3D <float> _ep = { -5, -5, -5 };

    EXPECT_EQ(dir, _dir);
    EXPECT_EQ(pos, _pos);
    EXPECT_EQ(ep, _ep);
}

TEST(vector3d_test, multiply)
{
    Vector3D vec1 = { { 3, 4, 5 }, { 10, 0, 0 }, 15 };
    Vector3D vec2 = { { 3, 4, 5 }, { 0, 6, 0 }, 10 };
    Vector3D vec3 = vec1 * vec2;
    Point3D <float> dir = vec3.getDirection();
    Point3D <float> pos = vec3.getPosition();
    Point3D <float> ep = vec3.getEndpoint();
    float len = vec3.getLength();

    Point3D <float> _dir = { 0, 0, 1 };
    Point3D <float> _pos = { 3, 4, 5 };
    Point3D <float> _ep = { 3, 4, 155 };
    float _len = 150;

    EXPECT_EQ(dir, _dir);
    EXPECT_EQ(pos, _pos);
    EXPECT_EQ(ep, _ep);
    EXPECT_EQ(len, _len);
}

TEST(vector3d_test, plus)
{
    Vector3D vec1 = { { 3, 4, 5 }, { 10, 0, 0 }, 15 };
    Vector3D vec2 = { { 3, 4, 5 }, { 0, 6, 0 }, 10 };
    Vector3D vec3 = vec1 + vec2;
    Point3D <float> dir = vec3.getDirection();
    Point3D <float> pos = vec3.getPosition();
    Point3D <float> ep = vec3.getEndpoint();
    float len = vec3.getLength();

    Point3D <float> _dir = { 3 * sqrt(13) / 13, 2 * sqrt(13) / 13, 0 };
    Point3D <float> _pos = { 3, 4, 5 };
    Point3D <float> _ep = { 18, 14, 5 };
    float _len = 5 * sqrt(13);

    EXPECT_EQ(dir, _dir);
    EXPECT_EQ(pos, _pos);
    EXPECT_EQ(ep, _ep);
    EXPECT_EQ(len, _len);
}

TEST(vector3d_test, minus)
{
    Vector3D vec1 = { { 3, 4, 5 }, { 10, 0, 0 }, 15 };
    Vector3D vec2 = { { 3, 4, 5 }, { 0, 6, 0 }, 10 };
    Vector3D vec3 = vec1 - vec2;
    Point3D <float> dir = vec3.getDirection();
    Point3D <float> pos = vec3.getPosition();
    Point3D <float> ep = vec3.getEndpoint();
    float len = vec3.getLength();

    Point3D <float> _dir = { 3 * sqrt(13) / 13, - 2 * sqrt(13) / 13, 0 };
    Point3D <float> _pos = { 3, 4, 5 };
    Point3D <float> _ep = { 18, -6, 5 };
    float _len = 5 * sqrt(13);

    EXPECT_EQ(dir, _dir);
    EXPECT_EQ(pos, _pos);
    EXPECT_EQ(ep, _ep);
    EXPECT_EQ(len, _len);
}
