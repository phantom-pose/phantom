#include "slice.h"

// Конструктор со стандартными пропорциями 1:1

Slice::Slice(int sizeX, int sizeY)
    : m_xSize(sizeX), m_ySize(sizeY)
{
    fillByZeroes();
}

Slice::Slice(int sizeX, int sizeY, float propX, float propY)
    : m_xSize(sizeX), m_ySize(sizeY), m_xProp(propX), m_yProp(propY)
{
    fillByZeroes();
}

Slice::fillByZeroes()
{
    m_data = new unsigned char *[m_ySize];
    for (int iy = 0; iy < m_ySize; iy++)
    {
        m_data[iy] = new unsigned char[m_xSize];
        for (int ix = 0; ix < m_xSize; ix++)
        {
            m_data[iy][ix] = 0;
        }
    }
}
