#include "knee.h"

BezierCoords3D * Knee::choose(float a1, float a2, float b1, float b2, float l1, float l2)
{
    return new BezierCoords3D(a1 > a2 ? a2 : a1, b1, l1/(l1+l2));
}

BoxNet GetKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord, bool right) {

    std::string partname = right ? "rightKnee" : "leftKnee";

    Json::Value rootJoint;
    std::ifstream jointfile("data/jointParams.json", std::ifstream::binary);
    jointfile >> rootJoint;
    jointfile.close();
    int KNEE_X1 = rootJoint[partname]["x1"].asInt();
    int KNEE_Y1 = rootJoint[partname]["y1"].asInt();
    int KNEE_BOTTOM_Z = rootJoint[partname]["z1"].asInt();
    int KNEE_X2 = rootJoint[partname]["x2"].asInt();
    int KNEE_Y2 = rootJoint[partname]["y2"].asInt();
    int KNEE_TOP_Z    = rootJoint[partname]["z2"].asInt();
    int indexLegRP    = rootJoint[partname]["rot1"].asInt();
    int indexKneeRP   = rootJoint[partname]["rot2"].asInt();

    Json::Value rootRot;
    std::ifstream rotfile("data/rotPoints.json", std::ifstream::binary);
    rotfile >> rootRot;
    rotfile.close();
    Json::Value const & points = rootRot;
    int ROT_KNEE_X = points[indexKneeRP]["xyz"][0].asInt();
    int ROT_KNEE_Y = points[indexKneeRP]["xyz"][1].asInt();
    int ROT_KNEE_Z = points[indexKneeRP]["xyz"][2].asInt();
    int ROT_LEG_X  = points[indexLegRP]["xyz"][0].asInt();
    int ROT_LEG_Y  = points[indexLegRP]["xyz"][1].asInt();
    int ROT_LEG_Z  = points[indexLegRP]["xyz"][2].asInt();

    const float dx = 0, dy =150, dz=50;
    const float xmax = 100;
    const float ymax = 370;
    const float zmax = 230;
    Knee knee = {
        {(dx+ROT_LEG_X-KNEE_X1)*VOX_X,(ROT_LEG_Y-KNEE_Y1+dy)*VOX_Y,(ROT_LEG_Z-KNEE_BOTTOM_Z+dz)*VOX_Z},
        {(dx+ROT_KNEE_X-KNEE_X1)*VOX_X, (ROT_KNEE_Y-KNEE_Y1+dy)*VOX_Y, (ROT_KNEE_Z-KNEE_BOTTOM_Z+dz)*VOX_Z},
        {dx*VOX_X, dy*VOX_Y, (KNEE_TOP_Z-KNEE_BOTTOM_Z+dz)*VOX_Z},
        (KNEE_X2-KNEE_X1)*VOX_X,
        (KNEE_Y2-KNEE_Y1)*VOX_Y,
        (KNEE_TOP_Z-KNEE_BOTTOM_Z)*VOX_Z,
        phi,
        theta

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
    BoxNet b = b1.cut( {KNEE_X1, KNEE_Y1, KNEE_BOTTOM_Z}, {KNEE_X2, KNEE_Y2, KNEE_TOP_Z} );
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

    *coord = {KNEE_X1-dx+xshift, KNEE_Y1-dy+yshift, KNEE_BOTTOM_Z - dz+zshift};
//    b2.setPosition(*coord);
    b2.shiftPos(*coord);
    return b2;
}

BoxNet RightKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord) {
    return GetKnee(b1, phi, theta, coord, true);
}

BoxNet LeftKnee(BoxNet b1, float phi, float theta, Point3D <int> * coord) {

    return GetKnee(b1, phi, theta, coord, false);
}
