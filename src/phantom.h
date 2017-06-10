#pragma once

#include "boxnet.h"
#include "slice.h"
#include "point3d.hpp"

/*!
 * \brief Основной класс для работы с воксельной моделью - Phantom
 */

class Phantom
{
public:
    Phantom();
    Phantom( BoxNet const & box );

    unsigned char getValue(int num);
    unsigned char getValue(int x, int y, int z);
    void setValue(int num, unsigned char value);
    void setValue(int x, int y, int z, unsigned char value);

    Slice getSliceX(int num) const;
    Slice getSliceY(int num) const;
    Slice getSliceZ(int num) const;

    int const & boxSizeX() const;
    int const & boxSizeY() const;
    int const & boxSizeZ() const;

    float const & xScale() const;
    float const & yScale() const;
    float const & zScale() const;

    BoxNet const & boxNet() const;

    void transliterate( Point3D <int> const & sizes, Point3D <int> const & position );

private:
    BoxNet m_boxNet;
    float m_xScale = 1.875;
    float m_yScale = 1.875;
    float m_zScale = 2.50;
};

std::ostream & operator << (std::ostream & os, Phantom const & obj);
