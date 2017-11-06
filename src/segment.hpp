#pragma once

class Segment
{
public:
    Segment(float const & l, float const & p, unsigned char const & c)
        : len(l), pos(p), col(c)
    {}
    Segment(float const & p, float const & e)
        : pos(p), end(e)
    {}
    float len;
    float pos;
    float end;
    unsigned char col;
};
