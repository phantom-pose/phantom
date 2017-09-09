#pragma once
#include "line.h"
#include <cmath>
#include <vector>
#include "segment.hpp"
#include "point3d.hpp"

class Line
{
public:
    Line(float xPos, float yPos, float zPos, float xDir, float yDir, float zDir);
    Line(Point3D <float> pos, Point3D <float> dir);
    std::vector <Segment> segments;
    Point3D <float> getPosition() const;
    Point3D <float> getDirection() const;
    void addSegment(float const & s, unsigned char const & col);
    void shiftPosition(float const & delta);
    void setMaxLen(float const & maxlen);
    void setParallel(int const & parallel);
    void setParallel();
    float getMaxLen() const;
    int getParallel() const;
    bool hasParallel() const;
private:
    void normalization();

    float m_x0 = 0.0f, m_y0 = 0.0f, m_z0 = 0.0f;
    float m_x = 0.0f, m_y = 0.0f, m_z = 1.0f;
    float m_llen = 0.0f; // last len
    float m_maxLen = 0.0f;
    int m_parallel = -1;
};

std::ostream & operator << (std::ostream & os, Line const & obj);
