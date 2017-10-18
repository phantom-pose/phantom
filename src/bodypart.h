#pragma once

#include <vector>
#include <fstream>
#include "point3d.hpp"
#include "rotationmatrix.h"
#include "cylinder.h"

class BodyPart
{
public:
    BodyPart();
    ~BodyPart();
    BodyPart(char const * filename);
    void fillData(char const * filename);
//    std::vector <int> & getData();
//    int next();
    std::vector <int> data;
    std::vector <RotationMatrix> matrices;
//    Point3D <float> getRotPoint() const;
//    void setRotPoint( Point3D <float> const & rotPoint );
    void setMatrix( RotationMatrix const & matrix );
    void setPrimitive( float x0, float y0, float z0, float a, float b, float h );
    void rotatePrimitive();
//    Point3D <float> getRotPoint();
//    std::vector <RotationMatrix> const & getMatrices();
//    RotationMatrix getMatrix() const;

private:

//    Point3D <float> m_rotPoint1 = {};
//    Point3D <float> m_rotPoint2 = {};
    Point3D <float> m_rotPoint;
    RotationMatrix m_matrix;
    bool m_matrixExist = false;
    Cylinder * m_primitive = nullptr;
};

//std::ostream & operator << (std::ostream & os, BodyPart const & obj);
