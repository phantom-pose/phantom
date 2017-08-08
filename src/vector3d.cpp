#include "vector3d.h"

Vector3D::Vector3D(Point3D <float> direction)
    : m_x( direction.x() ), m_y( direction.y() ), m_z( direction.z() )
{
    normalization();
    calculateEndpoint();
}

Vector3D::Vector3D(float const & dirX, float const & dirY, float const & dirZ)
    : m_x(dirX), m_y(dirY), m_z(dirZ), m_x1(dirX), m_y1(dirY), m_z1(dirZ)
{
    normalization();
    calculateEndpoint();
}

Vector3D::Vector3D(Point3D <float> position, Point3D <float> direction)
    : m_x( direction.x() ), m_y( direction.y() ), m_z( direction.z() ),
      m_x0( position.x() ), m_y0( position.y() ), m_z0( position.z() )
{
    normalization();
    calculateEndpoint();
}

Vector3D::Vector3D(Point3D <float> position, Point3D <float> direction, float const & len)
    : m_x( direction.x() ), m_y( direction.y() ), m_z( direction.z() ),
      m_x0( position.x() ), m_y0( position.y() ), m_z0( position.z() ), m_len(len)
{
    normalization();
    calculateEndpoint();
}

void Vector3D::setPosition(float const & x, float const & y, float const & z)
{
    m_x0 = x;
    m_y0 = y;
    m_z0 = z;
    calculateEndpoint();
}

void Vector3D::setDirection(float const & x, float const & y, float const & z)
{
    m_x = x;
    m_y = y;
    m_z = z;
    normalization();
    calculateEndpoint();
}

void Vector3D::setEndpoint(float const & x, float const & y, float const & z)
{
    m_x1 = x;
    m_y1 = y;
    m_z1 = z;

    m_x = m_x1 - m_x0;
    m_y = m_y1 - m_y0;
    m_z = m_z1 - m_z0;
    m_len = sqrt( m_x*m_x + m_y*m_y + m_z*m_z );
    normalization();
}

void Vector3D::setLength(float const & len)
{
    m_len = len;
    calculateEndpoint();
}

void Vector3D::setPosition(Point3D <float> const & position)
{
    setPosition( position.x(), position.y(), position.z() );
}

void Vector3D::setDirection(Point3D <float> const & direction)
{
    setDirection( direction.x(), direction.y(), direction.z() );
}

void Vector3D::setEndpoint(Point3D <float> const & endpoint)
{
    setEndpoint( endpoint.x(), endpoint.y(), endpoint.z() );
}

Point3D <float> Vector3D::getPosition() const
{
    return { m_x0, m_y0, m_z0 };
}
Point3D <float> Vector3D::getDirection() const
{
    return { m_x, m_y, m_z };
}
Point3D <float> Vector3D::getEndpoint() const
{
    return { m_x1, m_y1, m_z1 };
}
float Vector3D::getLength() const
{
    return m_len;
}

bool Vector3D::operator == (Vector3D const & obj) const
{
    Point3D <float> p1 = { m_x0, m_y0, m_z0 };
    Point3D <float> d1 = { m_x, m_y, m_z };
    Point3D <float> ep1 = { m_x1, m_y1, m_z1 };
    Point3D <float> p2 = { obj.m_x0, obj.m_y0, obj.m_z0 };
    Point3D <float> d2 = { obj.m_x, obj.m_y, obj.m_z };
    Point3D <float> ep2 = { obj.m_x1, obj.m_y1, obj.m_z1 };
    return ( p1 == p2 && d1 == d2 && m_len == obj.m_len && ep1 == ep2 );
}

Vector3D & Vector3D::operator = ( Vector3D const & obj )
{
    if ( this == &obj ) return *this;
    m_x0 = obj.m_x0;
    m_y0 = obj.m_y0;
    m_z0 = obj.m_z0;

    m_x = obj.m_x;
    m_y = obj.m_y;
    m_z = obj.m_z;

    m_x1 = obj.m_x1;
    m_y1 = obj.m_y1;
    m_z1 = obj.m_z1;

    m_len = obj.m_len;
    return *this;
}

Vector3D Vector3D::operator * (Vector3D const & obj) const
{
    Point3D <float> pos = { m_x0, m_y0, m_z0 };
    Vector3D res = { -1, -1, -1 };
    if (pos != obj.getPosition()) { return res; }
    float ax = m_x1 - m_x0;
    float ay = m_y1 - m_y0;
    float az = m_z1 - m_z0;
    Point3D <float> objPos = obj.getPosition();
    Point3D <float> objEp = obj.getEndpoint();
    float bx = objEp.x() - objPos.x();
    float by = objEp.y() - objPos.y();
    float bz = objEp.z() - objPos.z();

    float epX = ay * bz - az * by;
    float epY = az * bx - ax * bz;
    float epZ = ax * by - ay * bx;
    Point3D <float> ep = { epX, epY, epZ };
    res.setPosition( pos );
    res.setEndpoint( ep + pos );
    return res;
}

void Vector3D::calculateEndpoint()
{
    m_x1 = m_x0 + m_x * m_len;
    m_y1 = m_y0 + m_y * m_len;
    m_z1 = m_z0 + m_z * m_len;
}

void Vector3D::normalization()
{
    float hyp = sqrt( m_x * m_x + m_y * m_y + m_z * m_z );
    m_x /= hyp;
    m_y /= hyp;
    m_z /= hyp;
}
