#pragma once

#include "joint.h"
#include "point3d.hpp"

class Shoulder: public Joint {

public:
    Shoulder(Point3D <float> rot, Point3D <float> p1, float dy1, float dz1, Point3D <float> p2, float dx2, float dy2, float phi, float theta);

    virtual bool getStartPoint(Point3D <float> * end, Point3D <float> * start, float der) override;


private:

    virtual BezierCoords3D * choose(float a1, float a2, float b1, float, float l1, float l2) override;

    virtual Vector3D getNEndTop() override {
        return this->m_endPlane1.getN();
    }

    virtual Vector3D getNStartTop() override {
        return this->m_startPlane1.getN();
    }

    float COEF = 0.3;
};

BoxNet RightShoulder(BoxNet b1, float phi, float theta, Point3D <int> * coord);
