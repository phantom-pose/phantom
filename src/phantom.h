#pragma once

#include "boxnet.h"
#include "slice.h"
#include "point3d.hpp"
#include <vector>
#include <algorithm>
#include "bodypart.h"
#include <memory>
#include "rotationmatrix.h"

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
    unsigned char getValue(Point3D <float> point);
    void setValue(int num, unsigned char value);
    void setValue(int x, int y, int z, unsigned char value);
    void setValue(Point3D <float> point, unsigned char value);

    Point3D <float> center(int num) const;
    Point3D <float> center(Point3D <int> const & pos) const;
    Point3D <float> center(Point3D <float> const & pos) const;
    Point3D <float> quarterOne(int num) const;
    Point3D <float> quarterTwo(int num) const;
    Point3D <int> getXYZ(Point3D <float> const & point) const;

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

    void pickRightLeg();
    void pickLeftLeg();
    void pickRightHand();
    void pickLeftHand();

    void grow( Point3D <int> const & sizes, Point3D <int> const & position );
    void writeBinFile(char const * filename, std::vector<int> const & v);
    void readBin(char const * filename);
    void checkBin(char const * filename);
    void check(BodyPart const & bp);
    void cutBin(char const * filename, int firstEdge, int secondEdge);

//    void rotateBodyPart(BodyPart const & bp, float pitch, float yaw);
    void makeNet();
    void rotate();

private:
    BoxNet m_boxNet;
    float m_xScale = 1.875;
    float m_yScale = 1.875;
    float m_zScale = 2.50;
    Point3D <float> m_scales = { 1.875, 1.875, 2.50 };
//    std::vector<int> m_leftLeg1, m_leftLeg2, m_leftLeg, m_rightLeg, m_leftHand, m_rightHand;
    BodyPart * m_leftLeg1;
    std::vector <BodyPart*> m_bodyparts;
};

std::ostream & operator << (std::ostream & os, Phantom const & obj);
