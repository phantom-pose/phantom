#include "bodypart.h"

BodyPart::BodyPart()
{}

BodyPart::~BodyPart()
{
    for (auto pb = m_bboxes.begin(); pb != m_bboxes.end(); pb++) {
        delete *pb;
    }
}

BodyPart::BodyPart(char const * filename)
{
    fillData(filename);
}

void BodyPart::fillData(char const * filename)
{
    int num = 0;
    std::ifstream file(filename, std::ios::binary);
    if (file) {
        while (!file.eof())
        {
            file.read(reinterpret_cast<char*>(&num), sizeof(num));
            data.push_back(num);
        }
    }
    file.close();
}

void BodyPart::setMatrix( RotationMatrix const & matrix )
{
    matrices.push_back(matrix);
}

void BodyPart::setPrimitive( float x0, float y0, float z0, float a, float b, float c , std::string name)
{
    m_bboxes.push_back(new BoundingBox(x0, y0, z0, a, b, c, name));
}

void BodyPart::rotatePrimitive()
{
    for (auto pm = matrices.begin(); pm != matrices.end(); pm++) {
        RotationMatrix matrix = *pm;
        for (auto pb = m_bboxes.begin(); pb != m_bboxes.end(); pb++) {
            (*pb)->rotate(matrix);
        }
    }
}

void BodyPart::shiftPrimitivePosition( Point3D <float> const & shift )
{
    for (auto pb = m_bboxes.begin(); pb != m_bboxes.end(); pb++) {
        Point3D <float> pos = (*pb)->getPosition();
        pos = pos + shift;
        (*pb)->setPosition(pos);
    }
}

void BodyPart::appendToVector( std::vector <BoundingBox *> & obj )
{
    obj.insert(obj.end(), m_bboxes.begin(), m_bboxes.end());
}
