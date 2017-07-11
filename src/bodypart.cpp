#include "bodypart.h"

BodyPart::BodyPart()
{}

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

Point3D <float> BodyPart::getRotPoint() const { return m_rotPoint; }

void BodyPart::setRotPoint( Point3D <float> const & rotPoint )
{
    m_matrix.setRotPoint(rotPoint);
}

Point3D<float> BodyPart::getRotPoint()
{
    return m_matrix.getRotPoint();
}

void BodyPart::setMatrix( RotationMatrix const & matrix )
{
    if ( ! m_matrixExist ) {
        m_matrix = matrix;
        m_matrixExist = true;
    } else {
        m_matrix.multiply(matrix);
    }
}

RotationMatrix BodyPart::getMatrix() const
{
    if ( ! m_matrixExist ) {
        Logger::Instance() << "matrix is not exist\n";
    }
    return m_matrix;
}
