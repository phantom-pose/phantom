#pragma once
#include "logger.h"


class Slice
{
public:
    Slice(int sizeX, int sizeY);
    Slice(int sizeX, int sizeY, int propX, int propY);
    Slice(Slice && obj);

    Slice & operator=(Slice && obj);

    float getPropX();
    float getPropY();
    int getSizeX();
    int getSizeY();
    unsigned char getValue(int x, int y);

    void setValue(int x, int y, unsigned char value);

    ~Slice();
private:
    unsigned char ** m_data;
    int m_xSize, m_ySize;
    float m_xProp = 1.00f, m_yProp = 1.00f;

    void allocateMemory();
};
