#include "boxnet.h"
#include "logger.h"

BoxNet::BoxNet(int sizeX, int sizeY, int sizeZ)
    : m_xSize(sizeX), m_ySize(sizeY), m_zSize(sizeZ)
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
    int num = z * m_xSize * m_ySize + y * m_xSize + x;
    return m_list[num];
}

void BoxNet::setByNum(int num, unsigned char value)
{
    m_list[num] = value;
}

void BoxNet::setByXyz(int x, int y, int z, unsigned char value)
{
    int num = z * m_xSize * m_ySize + y * m_xSize + x;
    m_list[num] = value;
}

int BoxNet::getSizeX() { return m_xSize; }

int BoxNet::getSizeY() { return m_ySize; }

int BoxNet::getSizeZ() { return m_zSize; }

unsigned char ** BoxNet::getSliceY(int num) const
{
    if (num < 0 || num > m_ySize) return nullptr;
    unsigned char ** slice = new unsigned char *[m_zSize];
    for (int iz = 0; iz < m_zSize; iz++)
    {
        slice[iz] = new unsigned char[m_xSize];
        for (int ix = 0; ix < m_xSize; ix++)
        {
            int n = iz * m_xSize * m_ySize + num * m_xSize + ix;
            slice[iz][ix] = m_list[n];
        }
    }
    return slice;
}

unsigned char ** BoxNet::getSliceX(int num) const
{
    if (num < 0 || num > m_xSize) return nullptr;
    unsigned char ** slice = new unsigned char *[m_zSize];
    for (int iz = 0; iz < m_zSize; iz++)
    {
        slice[iz] = new unsigned char[m_ySize];
        for (int iy = 0; iy < m_ySize; iy++)
        {
            int n = iz * m_xSize * m_ySize + iy * m_xSize + num;
            slice[iz][iy] = m_list[n];
        }
    }
    return slice;
}

//unsigned char ** BoxNet::getSliceZ(int num) const
//{
//    if (num < 0 || num > m_zSize) return nullptr;
//    unsigned char ** slice = new unsigned char *[m_zSize];
//    for (int iy = 0; iy < m_ySize; iy++)
//    {
//        slice[iy] = new unsigned char[m_xSize];
//        for (int ix = 0; ix < m_xSize; ix++)
//        {
//            int n = num * m_xSize * m_ySize + iy * m_xSize + ix;
//            slice[iy][ix] = m_list[n];
//        }
//    }
//    return slice;
//}

Slice BoxNet::getSliceZ(int num) const
{
//    if (num < 0 || num > m_zSize) return nullptr;
    Slice slice = { m_xSize, m_ySize };

    for (int iy = 0; iy < m_ySize; iy++)
    {
        for (int ix = 0; ix < m_xSize; ix++)
        {
            int n = num * m_xSize * m_ySize + iy * m_xSize + ix;
            slice.setValue(ix, iy, m_list[n]);
        }
    }
    return slice;
}

void BoxNet::fillFromFile(char const * filename)
{
    std::ifstream f(filename);
    char line[100];
    int i = 0;
    int ival, n;
    if(f.is_open())
    {
        while (f.getline(line,100))
        {
            char *data = line;
            while (sscanf(data, "%d%n", &ival, &n) == 1)
            {
                data += n;
                m_list[i++] = (unsigned char)ival;
            }
        }
        f.close();
    } else {
        printf("Cant open file\n");
    }
}

void BoxNet::writeBinFile(char const * filename)
{
    int i = 0;
    std::ofstream file(filename, std::ios::binary);
    while (i < m_xSize * m_ySize * m_zSize)
    {
        file.write(reinterpret_cast<char const*>(&m_list[i]), sizeof(m_list[i]));
        i++;
    }
    file.close();
}

void BoxNet::fillFromBin(char const * filename)
{
    int i = 0;
    std::ifstream file(filename, std::ios::binary);
    while (!file.eof())
    {
        file.read(reinterpret_cast<char*>(&m_list[i]), sizeof(m_list[i]));
        i++;
    }
    file.close();
}
