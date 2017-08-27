#include "line.h"

Line::Line(float xPos, float yPos, float zPos, float xDir, float yDir, float zDir)
    :m_x0(xPos), m_y0(yPos), m_z0(zPos), m_x(xDir), m_y(yDir), m_z(zDir)
{}

Line::Line(Point3D <float> pos, Point3D <float> dir)
    :m_x0(pos.x()), m_y0(pos.y()), m_z0(pos.z()), m_x(dir.x()), m_y(dir.y()), m_z(dir.z())
{}

// Геттеры. обычные.
Point3D <float> Line::getPosition() const { return { m_x0, m_y0, m_z0 }; }
Point3D <float> Line::getDirection() const{ return { m_x, m_y, m_z }; }

void Line::addSegment(float const & len, unsigned char const & col)
{
    segments.push_back(Segment(len, m_llen, col));
    m_llen += len;
}

