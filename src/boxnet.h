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
    BoxNet();
    BoxNet(int sizeX, int sizeY, int sizeZ);
    BoxNet( BoxNet const & obj );
    ~BoxNet();
    unsigned char const & getByNum(int num) const;
    unsigned char const & getByXyz(int x, int y, int z) const;
    void setByNum(int num, unsigned char value);
    void setByXyz(int x, int y, int z, unsigned char value);

    bool operator == (BoxNet const & obj) const;

    int const & getSizeX() const;
    int const & getSizeY() const;
    int const & getSizeZ() const;

    int const & getLength() const;

    Slice getSliceX(int num) const;
    Slice getSliceY(int num) const;
    Slice getSliceZ(int num) const;

    void fillFromFile(char const * filename);
    void writeBinFile(char const * filename);
    void fillFromBin(char const * filename);

    void segmentation();
    BoxNet &  operator = ( BoxNet const & obj );

private:
    int m_xSize = 0, m_ySize = 0, m_zSize = 0, m_length = 0;
    double m_propX = 3.0, m_propY = 3.0, m_propZ = 8.0;
    unsigned char* m_list;
};

std::ostream & operator << (std::ostream & os, BoxNet const & obj);
