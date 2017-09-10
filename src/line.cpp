#include "line.h"

Line::Line(float xPos, float yPos, float zPos, float xDir, float yDir, float zDir)
    :m_x0(xPos), m_y0(yPos), m_z0(zPos), m_x(xDir), m_y(yDir), m_z(zDir)
{
    normalization();
    setParallel();
}

Line::Line(Point3D <float> pos, Point3D <float> dir)
    :m_x0(pos.x()), m_y0(pos.y()), m_z0(pos.z()), m_x(dir.x()), m_y(dir.y()), m_z(dir.z())
{
    normalization();
    setParallel();
}

// Геттеры. обычные.
Point3D <float> Line::getPosition() const { return { m_x0, m_y0, m_z0 }; }
Point3D <float> Line::getDirection() const{ return { m_x, m_y, m_z }; }

void Line::addSegment(float const & s, unsigned char const & col)
{
    float len = s - m_llen;
    segments.push_back(Segment(len, m_llen, col));
    m_llen = s;
}

void Line::shiftPosition(float const & delta)
{
    m_x0 += m_x * delta;
    m_y0 += m_y * delta;
    m_z0 += m_z * delta;
}

void Line::normalization()
{
    float hyp = sqrt( m_x * m_x + m_y * m_y + m_z * m_z );
    m_x /= hyp;
    m_y /= hyp;
    m_z /= hyp;
}

void Line::setMaxLen(float const & maxlen) { m_maxLen = maxlen; }
void Line::setParallel(int const & parallel) { m_pFactor = parallel; }
void Line::setParallel()
{
    if ( fabs(m_x) < kEps )
        m_pFactor += 1;
    if ( fabs(m_y) < kEps )
        m_pFactor += 2;
    if ( fabs(m_z) < kEps )
        m_pFactor += 4;
}

bool Line::hasParallel() const
{
    if (m_pFactor)
        return true;
    return false;
}

float Line::getMaxLen() const { return m_maxLen; }
int Line::getPFactor() const { return m_pFactor; }

std::ostream & operator << (std::ostream & os, Line const & obj)
{
    os << "|Line|\n\
        position  = {" << obj.getPosition() << "}\n\
        direction = {" << obj.getDirection() << "}\n\
        maxLen    = {" << obj.getMaxLen() << "}\n";
    int i = 0;
    for (auto it = obj.segments.begin(); it != obj.segments.end(); it++) {
        os << "segment "<< i << ": pos = " << (*it).pos << " len = " << (*it).len << " col = " << int((*it).col) << "\n";
        i++;
    }
    return os;
}
