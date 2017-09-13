#pragma once

#include "plane.h"
#include "point3d.hpp"
#include "iostream"

class Joint {
public:
    /*!
     * \brief Конструктор, передаём две начальные плоскости и две конечные,
     * плоскость 1 - нормаль ноправлена вне сустава в правой тройке
     */
    Joint( Plane * start1, Plane * start2, Plane * end1, Plane * end2 )
        :  m_startPlane1(start1), m_startPlane2(start2), m_endPlane1(end1), m_endPlane2(end2)
    {
        //std::cout << m_startPlane1->getN().getDirection().z() << "lol";
    }
    ~Joint()
    {}

    /*!
     * \brief поиск координаты в исходном положении плоскостей
     * Аргументы:
     * Point3D <float> * end - точка (координаты) в конечном положении (деформированный сустав)
     * Point3D <float> * start - инициализированная точка, в которую метод запишет точку в недеформированном суставе
     * Результат:
     * bool - принадлежит ли точка области деформации
     */
    bool getStartPoint( Point3D <float> * end, Point3D <float> * start);

private:
    Plane * m_startPlane1;
    Plane * m_startPlane2;
    Plane * m_endPlane1;
    Plane * m_endPlane2;
};
