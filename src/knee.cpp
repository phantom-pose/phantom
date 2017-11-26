#include "knee.h"

Knee::Knee(Point3D <float> rot1, Point3D <float> rot2, Point3D <float> start, float x, float y, float z, float theta, float phi)
{
    Plane startPlane1 = { start, {start.x(), start.y()+y, start.z()}, {start.x()+x, start.y(), start.z()} };
    Plane startPlane2 = { {start.x(), start.y(), start.z()-z}, {start.x()+x, start.y(), start.z()-z}, {start.x(), start.y()+y, start.z()-z} };
    Plane endPlane1 = {
        RotateX(rot1, start, theta),
        RotateX(rot1, {start.x(), start.y()+y, start.z()}, theta),
        RotateX(rot1, {start.x()+x, start.y(), start.z()}, theta)
    };

    Plane endPlane2 = {
        RotateX(rot1, RotateX(rot2, {start.x(), start.y(), start.z()-z}, -phi), theta),
        RotateX(rot1, RotateX(rot2, {start.x()+x, start.y(), start.z()-z}, -phi), theta),
        RotateX(rot1, RotateX(rot2, {start.x(), start.y()+y, start.z()-z}, -phi), theta)
    };

    m_joint = new Joint(startPlane1, startPlane2, endPlane1, endPlane2);
    Point3D <float> shift = endPlane1.getE1().getPosition() - startPlane1.getE1().getPosition();
    m_shift = new Vector3D(true, {0,0,0}, shift);
}

bool Knee::getStartPoint(Point3D <float> * end, Point3D <float> * start, float der)
{
    m_joint->getStartPoint(end, start, der);
}

constexpr int KNEE_TOP_Z = 240;
constexpr int KNEE_BOTTOM_Z = 200;

constexpr int ROT_RIGHT_LEG_X = 82;
constexpr int ROT_RIGHT_LEG_Y = 72;
constexpr int ROT_RIGHT_LEG_Z = 351;

constexpr int ROT_LEFT_LEG_X = 216;
constexpr int ROT_LEFT_LEG_Y = 72;
constexpr int ROT_LEFT_LEG_Z = 351;

constexpr int ROT_RIGHT_KNEE_X = 107;
constexpr int ROT_RIGHT_KNEE_Y = 83;
constexpr int ROT_RIGHT_KNEE_Z = 215;

constexpr int ROT_LEFT_KNEE_X = 187;
constexpr int ROT_LEFT_KNEE_Y = 83;
constexpr int ROT_LEFT_KNEE_Z = 215;

constexpr int RIGHT_KNEE_X1 = 66;
constexpr int RIGHT_KNEE_Y1 = 43;

constexpr int LEFT_KNEE_X1 = 148;
constexpr int LEFT_KNEE_Y1 = 43;

constexpr int RIGHT_KNEE_X2 = 144;
constexpr int RIGHT_KNEE_Y2 = 131;

constexpr int LEFT_KNEE_X2 = 227;
constexpr int LEFT_KNEE_Y2 = 131;

BoxNet RightKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord) {

    const float dx = 0, dy =140, dz=50;
    const float xmax = 100;
    const float ymax = 350;
    const float zmax = 200;
    Knee knee = {
        {(dx+ROT_RIGHT_LEG_X-RIGHT_KNEE_X1)*VOX_X,(ROT_RIGHT_LEG_Y-RIGHT_KNEE_Y1+dy)*VOX_Y,(ROT_RIGHT_LEG_Z-KNEE_BOTTOM_Z+dz)*VOX_Z},
        {(dx+ROT_RIGHT_KNEE_X-RIGHT_KNEE_X1)*VOX_X, (ROT_RIGHT_KNEE_Y-RIGHT_KNEE_Y1+dy)*VOX_Y, (ROT_RIGHT_KNEE_Z-KNEE_BOTTOM_Z+dz)*VOX_Z},
        {dx*VOX_X, dy*VOX_Y, (KNEE_TOP_Z-KNEE_BOTTOM_Z+dz)*VOX_Z},
        (RIGHT_KNEE_X2-RIGHT_KNEE_X1)*VOX_X,
        (RIGHT_KNEE_Y2-RIGHT_KNEE_Y1)*VOX_Y,
        (KNEE_TOP_Z-KNEE_BOTTOM_Z)*VOX_Z,
        theta,
        phi

    };
    float xshift, yshift, zshift;
    auto shift = knee.getShift();
    if (std::isnan(shift->getDirection().x()))
    {
        xshift=0;yshift=0;zshift=0;
    }
    else {
        xshift = int((shift->getDirection().x() * shift->getLength())/VOX_X);
        yshift = int((shift->getDirection().y() * shift->getLength())/VOX_Y);
        zshift = int((shift->getDirection().z() * shift->getLength())/VOX_Z);
    }
    BoxNet b = b1.cut( {RIGHT_KNEE_X1, RIGHT_KNEE_Y1, KNEE_BOTTOM_Z}, {RIGHT_KNEE_X2, RIGHT_KNEE_Y2, KNEE_TOP_Z} );
    b.grow({xmax,ymax,zmax},{dx,dy,dz});
    BoxNet b2 = {xmax,ymax,zmax};

    for (int iz = 0; iz < zmax; iz++)
    {
        std::cout << iz << std::endl;
        for (int iy = 0; iy < ymax; iy++)
        {
            //std::cout << iz << " " << iy<< std::endl;
            for (int ix = 0; ix < xmax; ix++)
            {
                //std::cout << iy << " " << ix<< std::endl;
                Point3D <float> end = { (ix+xshift+0.5)*VOX_X, (iy+yshift+0.5)*VOX_Y, (iz+zshift+0.5)*VOX_Z };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                bool hasDef = knee.getStartPoint( pend, pstart, 3 );
                float x = pstart->x();
                float y = pstart->y();
                float z = pstart->z();
                if (hasDef && x > 0 && x < xmax*VOX_X && y > 0 && y < ymax*VOX_Y && z > 0 && z < zmax*VOX_Z)
                {
                    b2.setByXyz(ix,iy,iz,b.getByXyz(int(x/VOX_X),int(y/VOX_Y),int(z/VOX_Z)));
                }
                else {
                    b2.setByXyz(ix, iy, iz, 0);
                }
            }
        }
    }

    *coord = {RIGHT_KNEE_X1-dx+xshift, RIGHT_KNEE_Y1-dy+yshift, KNEE_BOTTOM_Z - dz+zshift};

    return b2;
}

BoxNet LeftKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord) {

    const float dx = 0, dy =140, dz=50;
    const float xmax = 100;
    const float ymax = 350;
    const float zmax = 200;
    Knee knee = {
        {(dx+ROT_LEFT_LEG_X-LEFT_KNEE_X1)*VOX_X,(ROT_LEFT_LEG_Y-LEFT_KNEE_Y1+dy)*VOX_Y,(ROT_LEFT_LEG_Z-KNEE_BOTTOM_Z+dz)*VOX_Z},
        {(dx+ROT_LEFT_KNEE_X-LEFT_KNEE_X1)*VOX_X, (ROT_LEFT_KNEE_Y-LEFT_KNEE_Y1+dy)*VOX_Y, (ROT_LEFT_KNEE_Z-KNEE_BOTTOM_Z+dz)*VOX_Z},
        {dx*VOX_X, dy*VOX_Y, (KNEE_TOP_Z-KNEE_BOTTOM_Z+dz)*VOX_Z},
        (LEFT_KNEE_X2-LEFT_KNEE_X1)*VOX_X,
        (LEFT_KNEE_Y2-LEFT_KNEE_Y1)*VOX_Y,
        (KNEE_TOP_Z-KNEE_BOTTOM_Z)*VOX_Z,
        theta,
        phi

    };
    float xshift, yshift, zshift;
    auto shift = knee.getShift();
    if (std::isnan(shift->getDirection().x()))
    {
        xshift=0;yshift=0;zshift=0;
    }
    else {
        xshift = int((knee.getShift()->getDirection().x() * knee.getShift()->getLength())/VOX_X);
        yshift = int((knee.getShift()->getDirection().y() * knee.getShift()->getLength())/VOX_Y);
        zshift = int((knee.getShift()->getDirection().z() * knee.getShift()->getLength())/VOX_Z);
    }
    BoxNet b = b1.cut( {LEFT_KNEE_X1, LEFT_KNEE_Y1, KNEE_BOTTOM_Z}, {LEFT_KNEE_X2, LEFT_KNEE_Y2, KNEE_TOP_Z} );
    b.grow({xmax,ymax,zmax},{dx,dy,dz});
    BoxNet b2 = {xmax,ymax,zmax};

    for (int iz = 0; iz < zmax; iz++)
    {
        std::cout << iz << std::endl;
        for (int iy = 0; iy < ymax; iy++)
        {
            //std::cout << iz << " " << iy<< std::endl;
            for (int ix = 0; ix < xmax; ix++)
            {
                //std::cout << iy << " " << ix<< std::endl;
                Point3D <float> end = { (ix+xshift+0.5)*VOX_X, (iy+yshift+0.5)*VOX_Y, (iz+zshift+0.5)*VOX_Z };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                bool hasDef = knee.getStartPoint( pend, pstart, 3 );
                float x = pstart->x();
                float y = pstart->y();
                float z = pstart->z();
                if (hasDef && x > 0 && x < xmax*VOX_X && y > 0 && y < ymax*VOX_Y && z > 0 && z < zmax*VOX_Z)
                {
                    b2.setByXyz(ix,iy,iz,b.getByXyz(int(x/VOX_X),int(y/VOX_Y),int(z/VOX_Z)));
                }
                else {
                    b2.setByXyz(ix, iy, iz, 0);
                }
            }
        }
    }

    *coord = {LEFT_KNEE_X1-dx+xshift, LEFT_KNEE_Y1-dy+yshift, KNEE_BOTTOM_Z - dz+zshift};

    return b2;
}
