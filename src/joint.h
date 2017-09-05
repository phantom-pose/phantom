#pragma once

#include "plane.h"
#include "point3d.hpp"

class Joint {
public:
    Joint( Plane * start1, Plane * start2, Plane * end1, Plane * end2 )
        :  m_startPlane1(start1), m_startPlane2(start2), m_endPlane1(end1), m_endPlane2(end2)
    {}
    ~Joint();

    bool getStartPoint( Point3D <float> * end, Point3D <float> * start);

private:
    Plane * m_startPlane1;
    Plane * m_startPlane2;
    Plane * m_endPlane1;
    Plane * m_endPlane2;
};
