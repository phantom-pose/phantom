#pragma once

#include "plane.h"
#include "point3d.hpp"
#include "boxnet.h"

float constexpr VOX_X = 1.775;
float constexpr VOX_Y = 1.775;
float constexpr VOX_Z = 2.42;

/*!
 * \brief The BezierCoords3D class - координаты Безье (альфа, бета, t)
 */
class BezierCoords3D
{
public:
    BezierCoords3D(): m_alpha{-0.1}, m_beta{-0.1}, m_t{-1.0}
    {}

    BezierCoords3D(double a,double b, double t): m_alpha{a}, m_beta{b}, m_t{t}
    {}

    ~BezierCoords3D()
    {}

    double const & alpha() const { return m_alpha; }
    double const & beta() const { return m_beta; }
    double const & t() const { return m_t; }
    double m_alpha;
    double m_beta;
    double m_t;
};

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

protected:

    Point3D <float> * findPoint(BezierCoords3D * bc);

    BezierCoords3D * findAlpha(Point3D<float> * point, float der);

    virtual BezierCoords3D * startCoefs(Point3D<float> * point) = 0;

    virtual BezierCoords3D * choose(float a1, float a2, float b1, float b2, float l1, float l2) = 0;

    void funcNewt(float * x, float * fvec, float ** fjac, Point3D<float> * point);

    virtual Vector3D getNEndTop() = 0;
    virtual Vector3D getNStartTop() = 0;

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

Point3D <float> RotateX(Point3D <float> rot, Point3D <float>  start, float angle);
Point3D <float> RotateY(Point3D <float> rot, Point3D <float>  start, float angle);
