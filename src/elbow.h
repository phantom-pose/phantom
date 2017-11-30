#pragma once;
#include "recjoint.h"
#include "point3d.hpp"
#include "plane.h"
#include "boxnet.h"
#include "json/json.h"

class Elbow: public RectangleJoint
{
public:
    Elbow(Point3D <float> rot1, Point3D <float> rot2, Point3D <float> start, float x, float y,float z, float thetaX, float thetaY, float phi):
        RectangleJoint(rot1,rot2,start,x,y,z,phi,thetaX,thetaY)
    {}

private:
    virtual BezierCoords3D * choose(float a1, float a2, float b1, float b2, float l1, float l2) override;
};

BoxNet RightElbow(BoxNet b1, float phi, float thetaX, float thetaY, Point3D <int> * coord);
