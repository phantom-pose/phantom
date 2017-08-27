#pragma once

#include "boxnet.h"
#include "point3d.hpp"
#include <vector>
#include <algorithm>
#include "vector3d.h"
#include <limits>
#include "line.h"

class CalculationArea
{
public:
    CalculationArea(BoxNet const & box);

    unsigned char getValue(Point3D <float> point);
    void setBox(BoxNet const & box);

    bool hasInsideBox(Vector3D v);
    bool hasInsideBox(Point3D <float> const & pt);
    int findBeginPoint(Vector3D const & segment, Point3D <float> & pt);
    int linePlaneIntersect(float & len, Vector3D const & segment, Vector3D const & plane, Point3D <float> & pt);
    void startIterations(Line & line);

private:
    BoxNet m_boxNet;
    float m_xScale = 1.875;
    float m_yScale = 1.875;
    float m_zScale = 2.50;
    Point3D <float> m_boxSize;
};
