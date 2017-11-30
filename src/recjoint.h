#pragma once
#include "joint.h"
#include "point3d.hpp"
#include "vector3d.h"
#include "json/json.h"

class RectangleJoint: public Joint
{
public:
    RectangleJoint(Point3D <float> rot1, Point3D <float> rot2, Point3D <float> start, float x, float y, float z, float phi, float thetaX, float thetaY);

    Vector3D * getShift() { return m_shift; }
protected:
    virtual BezierCoords3D * startCoefs(Point3D<float> * point) override;

    virtual BezierCoords3D * choose(float a1, float a2, float b1, float b2, float l1, float l2) = 0;

    Vector3D * m_shift;
};
