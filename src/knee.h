#pragma once;
#include "joint.h"
#include "point3d.hpp"
#include "vector3d.h"
#include "plane.h"
#include "boxnet.h"
#include "json/json.h"

class Knee
{
public:
    Knee(Point3D <float> rot1, Point3D <float> rot2, Point3D <float> start, float x, float y,float z, float theta, float phi);

    Vector3D * getShift() { return m_shift; }

    bool getStartPoint(Point3D <float> * end, Point3D <float> * start, float der);

private:
    Joint * m_joint;
    Vector3D * m_shift;
};

BoxNet RightKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord);
BoxNet LeftKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord);

