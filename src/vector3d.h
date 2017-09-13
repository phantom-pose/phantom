#pragma once
#include "point3d.hpp"
#include <cmath>
#include "logger.h"

/*!
 * \brief The Vector3D class Класс представляет из себя вектор с началом в точке m_*0 и концом в точке m_*1
 * направление вектора задаётся тремя декартовыми числами, после чего он нормализуется.
 * Длина вектора не обязана быть единичной.
 */

class Vector3D
{
public:
    Vector3D() = default;
    Vector3D(Point3D <float> direction);
    Vector3D(float const & dirX, float const & dirY, float const & dirZ);
    Vector3D(Point3D <float> position, Point3D < float> direction);
    Vector3D(Point3D <float> position, Point3D < float> direction, float const & len);

    void setPosition(Point3D <float> const & position);
    void setPosition(float const & x, float const & y, float const & z);
    void setDirection(Point3D <float> const & direction);
    void setDirection(float const & x, float const & y, float const & z);
    void setEndpoint(float const & x, float const & y, float const & z);
    void setEndpoint(Point3D <float> const & endpoint);
    void setLength(float const & len);

    Point3D <float> getPosition() const;
    Point3D <float> getDirection() const;
    Point3D <float> getEndpoint() const;
    float getLength() const;

    bool operator == (Vector3D const & obj) const;
    Vector3D & operator = ( Vector3D const & obj );
    Vector3D operator * (Vector3D const & obj) const;

private:
    void calculateEndpoint();
    void normalization();

    float m_x0 = 0.0f, m_y0 = 0.0f, m_z0 = 0.0f;
    float m_x = 0.0f, m_y = 0.0f, m_z = 1.0f;
    float m_x1 = 0.0f, m_y1 = 0.0f, m_z1 = 1.0f;
    float m_len = 1.0f;
};

std::ostream & operator << (std::ostream & os, Vector3D const & obj);
