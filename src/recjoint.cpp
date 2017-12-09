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
