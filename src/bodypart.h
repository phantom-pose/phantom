#pragma once

#include <vector>
#include <fstream>
#include "point3d.hpp"
#include "rotationmatrix.h"
#include "cylinder.h"
#include "boundingbox.h"

class BodyPart
{
public:
    BodyPart();
    ~BodyPart();
    BodyPart(char const * filename);
    void fillData(char const * filename);
    std::vector <int> data;
    std::vector <RotationMatrix> matrices;
    void setMatrix( RotationMatrix const & matrix );
    void setPrimitive( float x0, float y0, float z0, float a, float b, float c );
    void rotatePrimitive();
    void shiftPrimitivePosition( Point3D <float> const & shift );
    void appendToVector( std::vector <BoundingBox *> & obj );

private:
//    BoundingBox * m_bboxes = nullptr;
    std::vector <BoundingBox *> m_bboxes;
};
