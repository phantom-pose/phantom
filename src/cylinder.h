#pragma once

#include "point3d.hpp"
#include "rotationmatrix.h"
#include "line.h"

class Cylinder
{
public:
    Cylinder( float x0, float y0, float z0, float a, float b, float h );
    void rotate(RotationMatrix matrix);
    int intersect(Line const & line, float & t1, float &t2);
    bool hasInsideWalls(float x, float y);
private:
    int quadratic(float a, float b, float c, float & t1, float & t2);
    // float m_x0 = 0, m_y0 = 0, m_z0 = 0;
    Point3D<float> m_pos = { 0, 0, 0 };
    Point3D<float> m_ex = { 1, 0, 0 };
    Point3D<float> m_ey = { 0, 1, 0 };
    Point3D<float> m_ez = { 0, 0, 1 };
    float m_h  = 1;
    float m_a = 1, m_b = 1;
};
