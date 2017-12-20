#include "elbow.h"

BezierCoords3D * Elbow::choose(float a1, float a2, float b1, float, float l1, float l2)
{
    return new BezierCoords3D(fabs(l1) > fabs(l2) ? a2 : a1, b1, l1/(l1+l2));
}


BoxNet GetElbow(BoxNet b1, float phi, float thetaX, float thetaY, Point3D <int> * coord, bool right) {

    std::string partname = right ? "rightElbow" : "leftElbow";

    Json::Value rootJoint;
    std::ifstream jointfile("data/jointParams.json", std::ifstream::binary);
    jointfile >> rootJoint;
    jointfile.close();
    const int ELBOW_X1  = rootJoint[partname]["x1"].asInt();
    const int ELBOW_Y1  = rootJoint[partname]["y1"].asInt();
    const int ELBOW_BOTTOM_Z  = rootJoint[partname]["z1"].asInt();
    const int ELBOW_X2  = rootJoint[partname]["x2"].asInt();
    const int ELBOW_Y2  = rootJoint[partname]["y2"].asInt();
    const int ELBOW_TOP_Z     = rootJoint[partname]["z2"].asInt();
    const int indexShoulderRP = rootJoint[partname]["rot1"].asInt();
    const int indexElbowRP    = rootJoint[partname]["rot2"].asInt();

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

    const int dx = 90, dy = 90, dz=15;
    const int xmax = 185;
    const int ymax = 185;
    const int zmax = 120;

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
    int xshift, yshift, zshift;
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
                Point3D <float> end = { (ix+0.5f+xshift)*VOX_X, (iy+0.5f+yshift)*VOX_Y, (iz+0.5f+zshift)*VOX_Z };
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
