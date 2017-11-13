#pragma once

class Segment
{
public:
    Segment() = default;
    Segment(float const & l, float const & p, unsigned char const & c)
        : len(l), pos(p), col(c)
    {}
    Segment(float const & p, float const & e)
        : pos(p), end(e), len(e - p)
    {}

    bool operator < (Segment const & obj) const
    {
        return pos < obj.pos;
    }

    float len;
    float pos;
    float end;
    unsigned char col = 0;
};
