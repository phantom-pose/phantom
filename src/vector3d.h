#pragma once
#include "point3d.hpp"

class Vector3D
{
public:
    Vector3D() = default;
    Vector3D(Point3D < float> direction);
    Vector3D(float const & dirX, float const & dirY, float const & dirZ);
    Vector3D(Point3D <float> position, Point3D < float> direction);

    void setPosition(Point3D <float> const & position);
    void setPosition(float const & x, float const & y, float const & z);
    void setDirection(Point3D <float> const & direction);
    void setDirection(float const & x, float const & y, float const & z);

    Point3D <float> getPosition() const;
    Point3D <float> getDirection() const;

    bool operator == (Vector3D const & obj) const;
    Vector3D &  operator = ( Vector3D const & obj );
private:
    float m_x0 = 0.0f, m_y0 = 0.0f, m_z0 = 0.0f;
    float m_x = 0.0f, m_y = 0.0f, m_z = 1.0f;
};
