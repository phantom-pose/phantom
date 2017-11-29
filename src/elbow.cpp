#include "elbow.h"

Elbow::Elbow(Point3D <float> rot1, Point3D <float> rot2, Point3D <float> start, float x, float y, float z, float thetaX, float thetaY, float phi):
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


BoxNet GetElbow(BoxNet b1, float phi, float thetaX, float thetaY, Point3D <int> * coord, bool right) {

    std::string partname = right ? "rightElbow" : "leftElbow";

    Json::Value rootJoint;
    std::ifstream jointfile("data/jointParams.json", std::ifstream::binary);
    jointfile >> rootJoint;
    jointfile.close();
    int ELBOW_X1  = rootJoint[partname]["x1"].asInt();
    int ELBOW_Y1  = rootJoint[partname]["y1"].asInt();
    int ELBOW_BOTTOM_Z  = rootJoint[partname]["z1"].asInt();
    int ELBOW_X2  = rootJoint[partname]["x2"].asInt();
    int ELBOW_Y2  = rootJoint["rightElbow"]["y2"].asInt();
    int ELBOW_TOP_Z     = rootJoint["rightElbow"]["z2"].asInt();
    int indexShoulderRP = rootJoint["rightElbow"]["rot1"].asInt();
    int indexElbowRP    = rootJoint["rightElbow"]["rot2"].asInt();

    Json::Value rootRot;
    std::ifstream rotfile("data/rotPoints.json", std::ifstream::binary);
    rotfile >> rootRot;
    rotfile.close();
    Json::Value const & points = rootRot;
    int ROT_ELBOW_X = points[indexElbowRP]["xyz"][0].asInt();
    int ROT_ELBOW_Y = points[indexElbowRP]["xyz"][1].asInt();
    int ROT_ELBOW_Z = points[indexElbowRP]["xyz"][2].asInt();
    int ROT_HAND_X  = points[indexShoulderRP]["xyz"][0].asInt();
    int ROT_HAND_Y  = points[indexShoulderRP]["xyz"][1].asInt();
    int ROT_HAND_Z  = points[indexShoulderRP]["xyz"][2].asInt();

    const float dx = 90, dy = 90, dz=15;
    const float xmax = 185;
    const float ymax = 185;
    const float zmax = 120;

    Elbow elbow = {
        {(ROT_HAND_X-ELBOW_X1+dx)*VOX_X,(ROT_HAND_Y-ELBOW_Y1+dy)*VOX_Y,(ROT_HAND_Z-ELBOW_BOTTOM_Z+dz)*VOX_Z},
        {(ROT_ELBOW_X-ELBOW_X1+dx)*VOX_X, (ROT_ELBOW_Y-ELBOW_Y1+dy)*VOX_Y, (ROT_ELBOW_Z-ELBOW_BOTTOM_Z+dz)*VOX_Z},
        {dx*VOX_X, dy*VOX_Y, (ELBOW_TOP_Z-ELBOW_BOTTOM_Z+dz)*VOX_Z},
        (ELBOW_X2-ELBOW_X1)*VOX_Y,
        (ELBOW_Y2-ELBOW_Y1)*VOX_Y,
        (ELBOW_TOP_Z-ELBOW_BOTTOM_Z)*VOX_Z,
        thetaX,
        thetaY,
        phi
    };

    float xshift, yshift, zshift;
    auto shift = elbow.getShift();
    if (std::isnan(shift->getDirection().x()))
    {
        xshift=0;yshift=0;zshift=0;
    }
    else {
        xshift = int((shift->getDirection().x() * shift->getLength())/VOX_X);
        yshift = int((shift->getDirection().y() * shift->getLength())/VOX_Y);
        zshift = int((shift->getDirection().z() * shift->getLength())/VOX_Z);
    }

    BoxNet b = b1.cut( {ELBOW_X1, ELBOW_Y1, ELBOW_BOTTOM_Z}, {ELBOW_X2, ELBOW_Y2, ELBOW_TOP_Z} );
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
                Point3D <float> end = { (ix+0.5+xshift)*VOX_X, (iy+0.5+yshift)*VOX_Y, (iz+0.5+zshift)*VOX_Z };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                bool hasDef = elbow.getStartPoint( pend, pstart, 3 );
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

    *coord = {ELBOW_X1-dx+xshift, ELBOW_Y1-dy+yshift, ELBOW_BOTTOM_Z - dz+zshift};
//    b2.setPosition(*coord);
    b2.shiftPos(*coord);
    return b2;
}

BoxNet RightElbow(BoxNet b1, float phi, float thetaX, float thetaY, Point3D <int> * coord) {
    return GetElbow(b1,phi,thetaX, thetaY, coord, true);
}

BoxNet LeftElbow(BoxNet b1, float phi, float thetaX, float thetaY, Point3D <int> * coord) {
    return GetElbow(b1,phi,thetaX, thetaY, coord, false);
}
