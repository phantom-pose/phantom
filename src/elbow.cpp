#include "elbow.h"

Elbow::Elbow(Point3D <float> rot1, Point3D <float> rot2, Point3D <float> start, float x, float y, float z, float thetaX, float thetaY, float phi)
{
    Plane startPlane1 = { start, {start.x(), start.y()+y, start.z()}, {start.x()+x, start.y(), start.z()} };
    Plane startPlane2 = { {start.x(), start.y(), start.z()-z}, {start.x()+x, start.y(), start.z()-z}, {start.x(), start.y()+y, start.z()-z} };
    Plane endPlane1 = {
        RotateY(rot1, RotateX(rot1, start, thetaX), thetaY),
        RotateY(rot1, RotateX(rot1, {start.x(), start.y()+y, start.z()}, thetaX), thetaY),
        RotateY(rot1, RotateX(rot1, {start.x()+x, start.y(), start.z()}, thetaX), thetaY)
    };

    Plane endPlane2 = {
        RotateY(rot1, RotateX(rot1, RotateY(rot2, {start.x(), start.y(), start.z()-z}, -phi), thetaX), thetaY),
        RotateY(rot1, RotateX(rot1, RotateY(rot2, {start.x()+x, start.y(), start.z()-z}, -phi), thetaX), thetaY),
        RotateY(rot1, RotateX(rot1, RotateY(rot2, {start.x(), start.y()+y, start.z()-z}, -phi), thetaX), thetaY)
    };

    m_joint = new Joint(startPlane1, startPlane2, endPlane1, endPlane2);
}

bool Elbow::getStartPoint(Point3D <float> * end, Point3D <float> * start, float der)
{
    m_joint->getStartPoint(end, start, der);
}

constexpr int ELBOW_TOP_Z = 520;
constexpr int ELBOW_BOTTOM_Z = 430;

constexpr int ROT_RIGHT_HAND_X = 60;
constexpr int ROT_RIGHT_HAND_Y = 70;
constexpr int ROT_RIGHT_HAND_Z = 579;

//constexpr int ROT_LEFT_LEG_X = 82;
//constexpr int ROT_LEFT_LEG_Y = 72;
//constexpr int ROT_LEFT_LEG_Z = 351;

constexpr int ROT_RIGHT_ELBOW_X = 41;
constexpr int ROT_RIGHT_ELBOW_Y = 113;
constexpr int ROT_RIGHT_ELBOW_Z = 473;

//constexpr int ROT_LEFT_KNEE_X = 187;
//constexpr int ROT_LEFT_KNEE_Y = 83;
//constexpr int ROT_LEFT_KNEE_Z = 215;

constexpr int RIGHT_ELBOW_X1 = 20;
constexpr int RIGHT_ELBOW_Y1 = 76;

//constexpr int LEFT_KNEE_X1 = 148;
//constexpr int LEFT_KNEE_Y1 = 43;

constexpr int RIGHT_ELBOW_X2 = 70;
constexpr int RIGHT_ELBOW_Y2 = 132;

//constexpr int LEFT_KNEE_X2 = 227;
//constexpr int LEFT_KNEE_Y2 = 131;


BoxNet RightElbow(BoxNet b1, float phi, float thetaX, float thetaY, Point3D <int> * coord) {
    BoxNet b = b1.cut( {RIGHT_ELBOW_X1, RIGHT_ELBOW_Y1, ELBOW_BOTTOM_Z}, {RIGHT_ELBOW_X2, RIGHT_ELBOW_Y2, ELBOW_TOP_Z} );
    const float xmax = 400;
    const float ymax = 400;
    const float zmax = 400;
    b.grow({xmax,ymax,zmax},{200,200,50});
    BoxNet b2 = {xmax,ymax,zmax};

    Elbow elbow = {
        {(ROT_RIGHT_HAND_X-RIGHT_ELBOW_X1)*1.875,(ROT_RIGHT_HAND_Y-RIGHT_ELBOW_Y1+200)*1.875,(ROT_RIGHT_HAND_Z-ELBOW_BOTTOM_Z+50)*2.5},
        {(ROT_RIGHT_ELBOW_X-RIGHT_ELBOW_X1)*1.875, (ROT_RIGHT_ELBOW_Y-RIGHT_ELBOW_Y1+200)*1.875, (ROT_RIGHT_ELBOW_Z-ELBOW_BOTTOM_Z+50)*2.5},
        {0, 200, (ELBOW_TOP_Z-ELBOW_BOTTOM_Z+50)*2.5},
        (RIGHT_ELBOW_X2-RIGHT_ELBOW_X1)*1.875,
        (RIGHT_ELBOW_Y2-RIGHT_ELBOW_Y1)*1.875,
        (ELBOW_TOP_Z-ELBOW_BOTTOM_Z)*2.5,
        thetaX,
        thetaY,
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
                bool hasDef = elbow.getStartPoint( pend, pstart, 3 );
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

    *coord = {RIGHT_ELBOW_X1, RIGHT_ELBOW_Y1-200, ELBOW_BOTTOM_Z - 50};

    return b2;
}
