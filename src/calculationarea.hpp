#pragma once

#include "boxnet.h"
#include "point3d.hpp"
#include <vector>
#include <algorithm>
#include "vector3d.h"
#include <limits>
#include "line.h"
#include "cjsonserializer.h"

class CalculationArea
{
public:
    CalculationArea(BoxNet const & box);

    unsigned char getValue(Point3D <float> point);
    void setBox(BoxNet const & box);

    bool hasInsideBox(Vector3D v);
    bool hasInsideBox(Point3D <float> const & pt);
    int searchIntersect(Line & line, double * tk, unsigned char * ck, int & k);
    int searchIntersectCostume(Line & line, double * tk, unsigned char * ck, int & k);
    int prepLineOut(Line & line);
    int prepLineIn(Line & line);
    int linePlaneIntersect(float & len, Line const & line, Vector3D const & plane);
    void startIterations(Line & line);
    void startParallelIterations(Line & line, int const & index, double * tk, unsigned char * ck, int & k);
    void startIterations(Line const & line, double * tk, unsigned char * ck, int & k);
    void startIterations(Line const & line, int fbIndex, double * tk, unsigned char * ck, int & k);

    int costumeIntersect(Line const & line, std::vector <Segment> & segments);


private:
    BoxNet m_boxNet;
    float m_xScale = 1.875;
    float m_yScale = 1.875;
    float m_zScale = 2.50;
    Point3D <float> m_boxSize;
    std::vector <Vector3D> m_planes; // Содержит 6 проскостей основной коробки
    std::vector <BoundingBox *> m_costume;
};
