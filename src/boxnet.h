#pragma once

#include <iostream>
#include <fstream>

class BoxNet
{
public:
    BoxNet(int sizeX, int sizeY, int sizeZ);
    ~BoxNet();
    unsigned char getByNum(int num);
    unsigned char getByXyz(int x, int y, int z);
    void setByNum(int num, unsigned char value);
    void setByXyz(int x, int y, int z, unsigned char value);
    int getSizeX();
    int getSizeY();
    int getSizeZ();
    unsigned char ** getSliceX(int num) const;
    unsigned char ** getSliceY(int num) const;
    unsigned char ** getSliceZ(int num) const;
    void fillFromFile(char const * filename);
    void writeBinFile(char const * filename);
    void fillFromBin(char const * filename);

private:
    int m_xSize, m_ySize, m_zSize;
    double m_propX = 3.0, m_propY = 3.0, m_propZ = 8.0;
    unsigned char* m_list;
};
