#pragma once

#include "point3d.hpp"

class Cylinder
{
public:
    Cylinder();
private:
    float m_x0 = 0, m_y0 = 0, m_z0 = 0;
    Point3D<float> m_ex = {};
    float m_h  = 1;
};
