#pragma once
#include "vector3d.h"

class Plane
{
public:
    // Конструктор по умолчанию
    Plane() = default;
    // Конструктор по позиции и двум конечным точкам
    Plane( Point3D <float> const & pos, Point3D <float> const & ep1, Point3D <float> const & ep2 );
    // Конструктор по позиции, двум направлениям и двум длинам
    Plane( Point3D <float> const & pos, Point3D <float> const & dir1, Point3D <float> const & dir2, float len1, float len2 );
    // Конструктор по двум векторам е1 е2
    Plane( Vector3D const & e1, Vector3D const & e2);
    // Геттеры
    Vector3D getN();
    Vector3D getE1();
    Vector3D getE2();
    float getLen1();
    float getLen2();
    Point3D <float> getPosition();

private:
    Vector3D m_e1 = { 1, 0, 0 };
    Vector3D m_e2 = { 0, 1, 0 };
};
