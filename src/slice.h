#pragma once


class Slice
{
public:
    Slice(int sizeX, int sizeY);
    Slice(int sizeX, int sizeY, float propX, float propY);

    void fillByZeroes();

    ~Slice();
private:
    unsigned char ** m_data;
    int m_xSize, m_ySize;
    float m_xProp = 1.00f, m_yProp = 1.00f;
};
