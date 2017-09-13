#include "plane.h"

// Конструктор по позиции и двум конечным точкам
Plane::Plane( Point3D <float> const & pos, Point3D <float> const & ep1, Point3D <float> const & ep2 )
{
//    std::cout << "IM HERE";
    m_e1.setPosition(pos);
//    std::cout << m_e1;
    m_e2.setPosition(pos);
    m_e1.setEndpoint(ep1);
//    std::cout << m_e1;
    m_e2.setEndpoint(ep2);
}

// Конструктор по позиции, двум направлениям и двум длинам
Plane::Plane( Point3D <float> const & pos, Point3D <float> const & dir1, Point3D <float> const & dir2, float len1, float len2 )
    : m_e1(pos, dir1, len1), m_e2(pos, dir2, len2)
{}

// Конструктор по двум векторам е1 е2
Plane::Plane( Vector3D const & e1, Vector3D const & e2)
    : m_e1(e1), m_e2(e2)
{}

// Геттеры
Vector3D Plane::getN()
{
    Vector3D norm = m_e1 * m_e2;
    norm.setLength(1);
    return norm;
}

Vector3D Plane::getE1() const { return m_e1; }
Vector3D Plane::getE2() const { return m_e2; }
float Plane::getLen1() { return m_e1.getLength(); }
float Plane::getLen2() { return m_e2.getLength(); }
Point3D <float> Plane::getPosition() { return m_e1.getPosition(); }

std::ostream & operator << (std::ostream & os, Plane const & obj)
{
    os << "|Plane|\n\
        E1 = " << obj.getE1() << "\n\
        E2 = " << obj.getE2() << "\n";
    return os;
}
