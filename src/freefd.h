#pragma once
#include "vector3d.h"


class FreeFD
{
public:
    FreeFD();
private:
    // три вектора определяющие первую плоскость
    Vector3D m_n1, m_ex1, m_ey1;
    // три вектора определяющие вторую плоскость
    Vector3D m_n2, m_ex2, m_ey2;
    // длины вдоль нормалей для плоскостей 1 и 2
    float m_lx1, m_ly1, m_lx2, m_ly2;
};
