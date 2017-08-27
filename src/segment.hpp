#pragma once

class Segment
{
public:
    Segment(float const & l, float const & p, unsigned char const & c)
        : len(l), pos(p), col(c)
    {}
    float len;
    float pos;
    unsigned char col;
};
