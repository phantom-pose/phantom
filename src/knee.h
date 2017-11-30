#pragma once;
#include "recjoint.h"
#include "point3d.hpp"
#include "vector3d.h"
#include "plane.h"
#include "boxnet.h"
#include "json/json.h"

class Knee: public RectangleJoint
{
public:
    Knee(Point3D <float> rot1, Point3D <float> rot2, Point3D <float> start, float x, float y, float z, float phi, float theta):
        RectangleJoint(rot1,rot2,start,x,y,z,-phi,theta,0)
    {}

private:

    virtual BezierCoords3D * choose(float a1, float a2, float b1, float b2, float l1, float l2) override;
};

BoxNet RightKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord);
BoxNet LeftKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord);

