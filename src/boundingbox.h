#pragma once

#include "point3d.hpp"
#include "rotationmatrix.h"
#include "line.h"

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(float xPos, float yPos, float zPos, float a, float b, float c);
    void setPosition(Point3D<float> const & pos);
    Point3D<float> const & getPosition() const;
    Point3D<float> const & getEx() const;
    Point3D<float> const & getEy() const;
    Point3D<float> const & getEz() const;
    Point3D<float> const & getPos(float h) const;
    Point3D<float> const & getEx(float h) const;
    Point3D<float> const & getEy(float h) const;
    void rotate(RotationMatrix matrix);
    int intersect(Line const & line, float & tmin, float & tmax);
    int validate(float const &t, int pFactor);


private:
    float m_a, m_b, m_c;
    Point3D<float> m_pos = { 0, 0, 0 };
    Point3D<float> m_ex = { 1, 0, 0 };
    Point3D<float> m_ey = { 0, 1, 0 };
    Point3D<float> m_ez = { 0, 0, 1 };
    // Относится к Line
    float m_x0, m_y0, m_z0;
    float m_xDir, m_yDir, m_zDir;
};

std::ostream & operator << (std::ostream & os, BoundingBox const & obj);
