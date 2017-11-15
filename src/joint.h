#pragma once

#include "plane.h"
#include "point3d.hpp"
#include "boxnet.h"
#include "phantom.h"

class Joint {
public:
    /*!
     * \brief Конструктор, передаём две начальные плоскости и две конечные,
     * плоскость 1 - альфа вдоль E1, бета вдоль E2, плоскость 1 - наоборот
     */
    Joint( Plane const & start1, Plane const & start2, Plane const & end1, Plane const & end2 )
        :  m_startPlane1(start1), m_startPlane2(start2), m_endPlane1(end1), m_endPlane2(end2)
    {}

    /*!
     * \brief поиск координаты Безье (альфа, бета, t) в исходном положении плоскостей
     * Аргументы:
     * Point3D <float> * end - точка (координаты) в конечном положении (деформированный сустав)
     * Point3D <float> * start - инициализированная точка, в которую метод запишет точку в недеформированном суставе
     * der - приемлимое отклонение (сумма квадратов по всем декартовым координатам)
     * Результат:
     * bool - принадлежит ли точка области деформации
     */
    bool getStartPoint(Point3D <float> * end, Point3D <float> * start, float der);

private:
    /*!
     * \brief m_startPlane1 - плоскость 1 в недеформированном суставе, альфа вдоль E1, бета вдоль E2
     */
    Plane m_startPlane1;
    /*!
     * \brief m_startPlane2 - плоскость 2 в недеформированном суставе, альфа вдоль E2, бета вдоль E1
     */
    Plane m_startPlane2;
    /*!
     * \brief m_endPlane1 - плоскость 1 в деформированном суставе, альфа вдоль E1, бета вдоль E2
     */
    Plane m_endPlane1;
    /*!
     * \brief m_endPlane2 - плоскость 2 в деформированном суставе, альфа вдоль E2, бета вдоль E1
     */
    Plane m_endPlane2;
};

class Knee
{
public:
    Knee(Point3D <float> rot1, Point3D <float> rot2, Point3D <float> start, float x, float y,float z, float theta, float phi);

    bool getStartPoint(Point3D <float> * end, Point3D <float> * start, float der);

private:
    Joint * m_joint;
};

BoxNet RightKnee(Phantom phantom, float phi, float theta, Point3D <int> * coord);
