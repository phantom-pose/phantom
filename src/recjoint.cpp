#include "recjoint.h"
#include "boxnet.h"

RectangleJoint::RectangleJoint(Point3D <float> rot1, Point3D <float> rot2, Point3D <float> start, float x, float y, float z, float phi, float thetaX, float thetaY):
    Joint(
            { start, {start.x(), start.y()+y, start.z()}, {start.x()+x, start.y(), start.z()} },
            { {start.x(), start.y(), start.z()-z}, {start.x()+x, start.y(), start.z()-z}, {start.x(), start.y()+y, start.z()-z} },
            {
                RotateY(rot1, RotateX(rot1, start, thetaX), thetaY),
                RotateY(rot1, RotateX(rot1, {start.x(), start.y()+y, start.z()}, thetaX), thetaY),
                RotateY(rot1, RotateX(rot1, {start.x()+x, start.y(), start.z()}, thetaX), thetaY)
            },
            {
                RotateY(rot1, RotateX(rot1, RotateX(rot2, {start.x(), start.y(), start.z()-z}, phi), thetaX), thetaY),
                RotateY(rot1, RotateX(rot1, RotateX(rot2, {start.x()+x, start.y(), start.z()-z}, phi), thetaX), thetaY),
                RotateY(rot1, RotateX(rot1, RotateX(rot2, {start.x(), start.y()+y, start.z()-z}, phi), thetaX), thetaY)
            }
        )
{
    Point3D <float> shift = m_endPlane1.getE1().getPosition() - m_startPlane1.getE1().getPosition();
    m_shift = new Vector3D(true, {0,0,0}, shift);
}

/*!
 * \brief startCoefs - ищет начальное приближение для (альфа, бета, t)
 * \param point
 * \return - начальные (альфа, бета, t)
 */
BezierCoords3D * RectangleJoint::startCoefs(Point3D<float> * point)
{
    auto n1 = m_endPlane1.getN();
    auto dir1 = n1.getDirection();
    float A1 = dir1.x(); float B1 = dir1.y(); float C1 = dir1.z();
    auto e1_1 = m_endPlane1.getE1(); auto e2_1 = m_endPlane1.getE2();
    auto ver1 = e1_1.getPosition();
    float D1 = -ver1.x()*A1 - ver1.y()*B1 - ver1.z()*C1;
    float lenn1_s = (A1*point->x() + B1*point->y() + C1*point->z() + D1) / sqrt(A1*A1+B1*B1+C1*C1);
    if (lenn1_s < 0) {
        return new BezierCoords3D(1,1,-1);
    }
    float lenn1 = fabs(lenn1_s);
    Point3D <float> pn1 = {point->x() - A1*lenn1, point->y() - B1*lenn1, point->z() - C1*lenn1};
    Vector3D v1 = {true, ver1, pn1};
    float a1 = (v1.getDirection().x()*e1_1.getDirection().x() +
            v1.getDirection().y()*e1_1.getDirection().y() +
            v1.getDirection().z()*e1_1.getDirection().z()) * v1.getLength()/e1_1.getLength();
    float b1 = (v1.getDirection().x()*e2_1.getDirection().x() +
            v1.getDirection().y()*e2_1.getDirection().y() +
            v1.getDirection().z()*e2_1.getDirection().z()) * v1.getLength()/e2_1.getLength();

    auto n2 = m_endPlane2.getN();
    auto dir2 = n2.getDirection();
    float A2 = dir2.x(); float B2 = dir2.y(); float C2 = dir2.z();
    auto e1_2 = m_endPlane2.getE2(); auto e2_2 = m_endPlane2.getE1();
    auto ver2 = e1_2.getPosition();
    float D2 = -ver2.x()*A2 - ver2.y()*B2 - ver2.z()*C2;
    float lenn2_s = (A2*point->x() + B2*point->y() + C2*point->z() + D2) / sqrt(A2*A2+B2*B2+C2*C2);
    if (lenn2_s < 0) {
        return new BezierCoords3D(1,1,-1);
    }
    float lenn2 = fabs(lenn2_s);
    Point3D <float> pn2 = {point->x() - A2*lenn2, point->y() - B2*lenn2, point->z() - C2*lenn2};
    Vector3D v2 = {true, ver2, pn2};
    float a2 = (v2.getDirection().x()*e1_2.getDirection().x() +
            v2.getDirection().y()*e1_2.getDirection().y() +
            v2.getDirection().z()*e1_2.getDirection().z()) * v2.getLength()/e1_2.getLength();
    float b2 = (v2.getDirection().x()*e2_2.getDirection().x() +
            v2.getDirection().y()*e2_2.getDirection().y() +
            v2.getDirection().z()*e2_2.getDirection().z()) * v2.getLength()/e2_2.getLength();
    return this->choose(a1,a2,b1,b2,lenn1_s,lenn2_s);
}
