#pragma once

#include <iostream>
#include <fstream>
#include "slice.h"

/*!
 * \brief Класс BoxNet - представление сетки и работа с ней
 *
 * Данный класс необходим для представления основной структуры Phantom - трехмерной сетки
 * и содержит основные операции, связанные с ней, как заполнение блока из файла или
 * получение значения ячейки
 */

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
    Slice getSliceX(int num) const;
    Slice getSliceY(int num) const;
    Slice getSliceZ(int num) const;

    void fillFromFile(char const * filename);
    void writeBinFile(char const * filename);
    void fillFromBin(char const * filename);

    void segmentation();

private:
    int m_xSize, m_ySize, m_zSize, m_length;
    double m_propX = 3.0, m_propY = 3.0, m_propZ = 8.0;
    unsigned char* m_list;
};
