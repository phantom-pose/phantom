#pragma once

#include "joint.h"
#include "point3d.hpp"

class Hip: public Joint {

public:
    Hip(Point3D <float> rot, Point3D <float> start, float x, float y, float z, float dz, float theta);

private:
    virtual BezierCoords3D * startCoefs(Point3D<float> * point) override;

    virtual BezierCoords3D * choose(float a1, float a2, float b1, float b2, float l1, float l2) override;

    virtual Vector3D getNEndTop() override {
        return {0,0,-1};
    }

    virtual Vector3D getNStartTop() override {
        return {0,0,-1};
    }
};

BoxNet RightHip(BoxNet b1, float theta, Point3D <int> * coord);
