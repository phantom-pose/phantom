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
}

bool Knee::getStartPoint(Point3D <float> * end, Point3D <float> * start, float der)
{
    m_joint->getStartPoint(end, start, der);
}

constexpr int KNEE_TOP_Z = 260;
constexpr int KNEE_BOTTOM_Z = 175;

constexpr int ROT_RIGHT_LEG_X = 82;
constexpr int ROT_RIGHT_LEG_Y = 72;
constexpr int ROT_RIGHT_LEG_Z = 351;

constexpr int ROT_LEFT_LEG_X = 82;
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
    BoxNet b = b1.cut( {RIGHT_KNEE_X1, RIGHT_KNEE_Y1, KNEE_BOTTOM_Z}, {RIGHT_KNEE_X2, RIGHT_KNEE_Y2, KNEE_TOP_Z} );
    const float xmax = 100;
    const float ymax = 400;
    const float zmax = 400;
    b.grow({xmax,ymax,zmax},{0,200,50});
    BoxNet b2 = {xmax,ymax,zmax};

    Knee knee = {
        {(ROT_RIGHT_LEG_X-RIGHT_KNEE_X1)*1.875,(ROT_RIGHT_LEG_Y-RIGHT_KNEE_Y1+200)*1.875,(ROT_RIGHT_LEG_Z-KNEE_BOTTOM_Z+50)*2.5},
        {(ROT_RIGHT_KNEE_X-RIGHT_KNEE_X1)*1.875, (ROT_RIGHT_KNEE_Y-RIGHT_KNEE_Y1+200)*1.875, (ROT_RIGHT_KNEE_Z-KNEE_BOTTOM_Z+50)*2.5},
        {0, 200, (KNEE_TOP_Z-KNEE_BOTTOM_Z+50)*2.5},
        (RIGHT_KNEE_X2-RIGHT_KNEE_X1)*1.875,
        (RIGHT_KNEE_Y2-RIGHT_KNEE_Y1)*1.875,
        (KNEE_TOP_Z-KNEE_BOTTOM_Z)*2.5,
        theta,
        phi

    };

    for (int iz = 0; iz < zmax; iz++)
    {
        std::cout << iz << std::endl;
        for (int iy = 0; iy < ymax; iy++)
        {
            //std::cout << iz << " " << iy<< std::endl;
            for (int ix = 0; ix < xmax; ix++)
            {
                //std::cout << iy << " " << ix<< std::endl;
                Point3D <float> end = { (ix+0.5)*1.875, (iy+0.5)*1.875, (iz+0.5)*2.5 };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                bool hasDef = knee.getStartPoint( pend, pstart, 3 );
                float x = pstart->x();
                float y = pstart->y();
                float z = pstart->z();
                if (hasDef && x > 0 && x < xmax*1.875 && y > 0 && y < ymax*1.875 && z > 0 && z < zmax*2.5)
                {
                    b2.setByXyz(ix,iy,iz,b.getByXyz(int(x/1.875),int(y/1.875),int(z/2.5)));
                }
                else {
                    b2.setByXyz(ix, iy, iz, 0);
                }
            }
        }
    }

    *coord = {RIGHT_KNEE_X1, RIGHT_KNEE_Y1-200, KNEE_BOTTOM_Z - 50};

    return b2;
}

BoxNet LeftKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord) {
    BoxNet b = b1.cut( {LEFT_KNEE_X1, LEFT_KNEE_Y1, KNEE_BOTTOM_Z}, {LEFT_KNEE_X2, LEFT_KNEE_Y2, KNEE_TOP_Z} );
    const float xmax = 100;
    const float ymax = 400;
    const float zmax = 400;
    b.grow({xmax,ymax,zmax},{0,200,50});
    BoxNet b2 = {xmax,ymax,zmax};

    Knee knee = {
        {(ROT_LEFT_LEG_X-LEFT_KNEE_X1)*1.875,(ROT_LEFT_LEG_Y-LEFT_KNEE_Y1+200)*1.875,(ROT_LEFT_LEG_Z-KNEE_BOTTOM_Z+50)*2.5},
        {(ROT_LEFT_KNEE_X-LEFT_KNEE_X1)*1.875, (ROT_LEFT_KNEE_Y-LEFT_KNEE_Y1+200)*1.875, (ROT_LEFT_KNEE_Z-KNEE_BOTTOM_Z+50)*2.5},
        {0, 200, (KNEE_TOP_Z-KNEE_BOTTOM_Z+50)*2.5},
        (LEFT_KNEE_X2-LEFT_KNEE_X1)*1.875,
        (LEFT_KNEE_Y2-LEFT_KNEE_Y1)*1.875,
        (KNEE_TOP_Z-KNEE_BOTTOM_Z)*2.5,
        theta,
        phi
    };

    for (int iz = 0; iz < zmax; iz++)
    {
        std::cout << iz << std::endl;
        for (int iy = 0; iy < ymax; iy++)
        {
            //std::cout << iz << " " << iy<< std::endl;
            for (int ix = 0; ix < xmax; ix++)
            {
                //std::cout << iy << " " << ix<< std::endl;
                Point3D <float> end = { (ix+0.5)*1.875, (iy+0.5)*1.875, (iz+0.5)*2.5 };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                bool hasDef = knee.getStartPoint( pend, pstart, 3 );
                float x = pstart->x();
                float y = pstart->y();
                float z = pstart->z();
                if (hasDef && x > 0 && x < xmax*1.875 && y > 0 && y < ymax*1.875 && z > 0 && z < zmax*2.5)
                {
                    b2.setByXyz(ix,iy,iz,b.getByXyz(int(x/1.875),int(y/1.875),int(z/2.5)));
                }
                else {
                    b2.setByXyz(ix, iy, iz, 0);
                }
            }
        }
    }

    *coord = {LEFT_KNEE_X1, LEFT_KNEE_Y1-200, KNEE_BOTTOM_Z - 50};

    return b2;
}
