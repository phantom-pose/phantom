#include "bodypart.h"

BodyPart::BodyPart()
{}

BodyPart::~BodyPart()
{
    delete m_primitive;
}

BodyPart::BodyPart(char const * filename)
{
    fillData(filename);
}

void BodyPart::fillData(char const * filename)
{
    int num = 0;
    std::ifstream file(filename, std::ios::binary);
    while (!file.eof())
    {
        file.read(reinterpret_cast<char*>(&num), sizeof(num));
        data.push_back(num);
    }
    file.close();
}

//Point3D <float> BodyPart::getRotPoint() const { return m_rotPoint; }

//void BodyPart::setRotPoint( Point3D <float> const & rotPoint )
//{
//    m_matrix.setRotPoint(rotPoint);
//}

//Point3D<float> BodyPart::getRotPoint()
//{
//    return m_matrix.getRotPoint();
//}

void BodyPart::setMatrix( RotationMatrix const & matrix )
{
    matrices.push_back(matrix);
}

void BodyPart::setPrimitive( float x0, float y0, float z0, float a, float b, float h )
{
    m_primitive = new Cylinder(x0, y0, z0, a, b, h);
}

void BodyPart::rotatePrimitive()
{
    if (m_primitive != nullptr) {
        for (auto pm = matrices.begin(); pm != matrices.end(); pm++) {
            RotationMatrix matrix = *pm;
            m_primitive->rotate(matrix);
        }
    }
}

//RotationMatrix BodyPart::getMatrix() const
//{
//    return m_matrix;
//}

//std::vector <RotationMatrix> const & BodyPart::getMatrices()
//{
//    return m_matrices;
//}

//std::ostream & operator << (std::ostream & os, BodyPart const & obj)
//{
//    for (auto matrix = obj.matrices.begin(); matrix != obj.matrices.end(); matrix++) {
////        os << *matrix;
//    }
//    return os;
//}
