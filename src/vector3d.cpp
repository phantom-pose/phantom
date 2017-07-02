#include "vector3d.h"

Vector3D::Vector3D(Point3D < float> direction)
    : m_x( direction.x() ), m_y( direction.y() ), m_z( direction.z() )
{}

Vector3D::Vector3D(float const & dirX, float const & dirY, float const & dirZ)
    : m_x(dirX), m_y(dirY), m_z(dirZ)
{}

Vector3D::Vector3D(Point3D <float> position, Point3D < float> direction)
    : m_x( direction.x() ), m_y( direction.y() ), m_z( direction.z() ),
      m_x0( position.x() ), m_y0( position.y() ), m_z0( position.z() )
{}

void Vector3D::setPosition(float const & x, float const & y, float const & z)
{
    m_x0 = x;
    m_y0 = y;
    m_z0 = z;
}

void Vector3D::setDirection(float const & x, float const & y, float const & z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void Vector3D::setPosition(Point3D <float> const & position)
{
    setPosition( position.x(), position.y(), position.z() );
}

void Vector3D::setDirection(Point3D <float> const & direction)
{
    setDirection( direction.x(), direction.y(), direction.z() );
}

Point3D <float> Vector3D::getPosition() const
{
    return { m_x0, m_y0, m_z0 };
}
Point3D <float> Vector3D::getDirection() const
{
    return { m_x, m_y, m_z };
}

bool Vector3D::operator == (Vector3D const & obj) const
{
    Point3D <float> p1 = { m_x0, m_y0, m_z0 };
    Point3D <float> d1 = { m_x, m_y, m_z };
    Point3D <float> p2 = { obj.m_x0, obj.m_y0, obj.m_z0 };
    Point3D <float> d2 = { obj.m_x, obj.m_y, obj.m_z };
    return ( p1 == p2 && d1 == d2 );
}

Vector3D &  Vector3D::operator = ( Vector3D const & obj )
{
    if ( this == &obj ) return *this;
    m_x0 = obj.m_x0;
    m_y0 = obj.m_y0;
    m_z0 = obj.m_z0;

    m_x = obj.m_x;
    m_y = obj.m_y;
    m_z = obj.m_z;
    return *this;
}
