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
    std::vector <RotationMatrix> matrices;
//    Point3D <float> getRotPoint() const;
//    void setRotPoint( Point3D <float> const & rotPoint );
    void setMatrix( RotationMatrix const & matrix );
//    Point3D <float> getRotPoint();
//    std::vector <RotationMatrix> const & getMatrices();
//    RotationMatrix getMatrix() const;

private:

//    Point3D <float> m_rotPoint1 = {};
//    Point3D <float> m_rotPoint2 = {};
    Point3D <float> m_rotPoint;
    RotationMatrix m_matrix;
    bool m_matrixExist = false;
};

//std::ostream & operator << (std::ostream & os, BodyPart const & obj);
