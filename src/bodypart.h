#pragma once

#include <vector>
#include <fstream>
#include "point3d.hpp"
#include "rotationmatrix.h"

class BodyPart
{
public:
    BodyPart();
    BodyPart(char const * filename);
    void fillData(char const * filename);
//    std::vector <int> & getData();
//    int next();
    std::vector <int> data;
    Point3D <float> getRotPoint() const;
    void setRotPoint( Point3D <float> const & rotPoint );
    void setMatrix( RotationMatrix const & matrix );
    RotationMatrix getMatrix() const;

private:

//    Point3D <float> m_rotPoint1 = {};
//    Point3D <float> m_rotPoint2 = {};
    Point3D <float> m_rotPoint;
    RotationMatrix m_matrix;
    bool m_matrixExist = false;
};
