#include "boxnet.h"

BoxNet::BoxNet(int sizeX, int sizeY, int sizeZ)
    : m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ)
{
    int length = sizeX * sizeY * sizeZ;
    m_list = new unsigned char[length];
    //fill array with zeroes
    for (int i = 0; i < length; i++) {
        m_list[i] = 0;
    }
}

BoxNet::~BoxNet()
{
    delete[] m_list;
}

unsigned char BoxNet::getByNum(int num)
{
    return m_list[num];
}

unsigned char BoxNet::getByXyz(int x, int y, int z)
{
    int num = z * m_sizeX * m_sizeY + y * m_sizeX + x;
    return m_list[num];
}

void BoxNet::setByNum(int num, unsigned char value)
{
    m_list[num] = value;
}

void BoxNet::setByXyz(int x, int y, int z, unsigned char value)
{
    int num = z * m_sizeX * m_sizeY + y * m_sizeX + x;
    m_list[num] = value;
}

int BoxNet::getSizeX() { return m_sizeX; }

int BoxNet::getSizeY() { return m_sizeY; }

int BoxNet::getSizeZ() { return m_sizeZ; }

unsigned char ** BoxNet::getSliceY(int num) const
{
    if (num < 0 || num > m_sizeY) return nullptr;
    unsigned char ** slice = new unsigned char *[m_sizeZ];
    for (int iz = 0; iz < m_sizeZ; iz++)
    {
        slice[iz] = new unsigned char[m_sizeX];
        for (int ix = 0; ix < m_sizeX; ix++)
        {
            int n = iz * m_sizeX * m_sizeY + num * m_sizeX + ix;
            slice[iz][ix] = m_list[n];
        }
    }
    return slice;
}

unsigned char ** BoxNet::getSliceX(int num) const
{
    if (num < 0 || num > m_sizeX) return nullptr;
    unsigned char ** slice = new unsigned char *[m_sizeZ];
    for (int iz = 0; iz < m_sizeZ; iz++)
    {
        slice[iz] = new unsigned char[m_sizeY];
        for (int iy = 0; iy < m_sizeY; iy++)
        {
            int n = iz * m_sizeX * m_sizeY + iy * m_sizeX + num;
            slice[iz][iy] = m_list[n];
        }
    }
    return slice;
}

unsigned char ** BoxNet::getSliceZ(int num) const
{
    if (num < 0 || num > m_sizeZ) return nullptr;
    unsigned char ** slice = new unsigned char *[m_sizeZ];
    for (int iy = 0; iy < m_sizeY; iy++)
    {
        slice[iy] = new unsigned char[m_sizeX];
        for (int ix = 0; ix < m_sizeX; ix++)
        {
            int n = num * m_sizeX * m_sizeY + iy * m_sizeX + ix;
            slice[iy][ix] = m_list[n];
        }
    }
    return slice;
}
