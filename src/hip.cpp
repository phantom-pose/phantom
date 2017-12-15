#include "hip.h"
#include "json/json.h"
#include <set>

const std::set<unsigned char> ARM_COLORS = {
    11, 19, 20, 21, 22, 23, 59, 104, 108, 118, 124
};

Hip::Hip(Point3D <float> rot, Point3D <float> start, float x, float y, float z, float dz, float theta):
    Joint(
            { start, {start.x(), start.y()+y, start.z()+dz}, {start.x()+x, start.y(), start.z()} },
            { {start.x(), start.y(), start.z()-z}, {start.x()+x, start.y(), start.z()-z}, {start.x(), start.y()+y, start.z()-z} },
            { start, {start.x(), start.y()+y, start.z()+dz}, {start.x()+x, start.y(), start.z()} },
            {
                RotateX(rot, {start.x(), start.y(), start.z()-z}, theta),
                RotateX(rot, {start.x()+x, start.y(), start.z()-z}, theta),
                RotateX(rot, {start.x(), start.y()+y, start.z()-z}, theta),
            }
        )
{}

BezierCoords3D * Hip::choose(float a1, float a2, float b1, float b2, float l1, float l2)
{
    return new BezierCoords3D(a1 > a2 ? a2 : a1, b1, l1/(l1+l2));
}

BoxNet GetHip(BoxNet b1, float theta, Point3D <int> * coord, bool right) {

    std::string partname = right ? "rightHip" : "leftHip";

    Json::Value rootJoint;
    std::ifstream jointfile("data/jointParams.json", std::ifstream::binary);
    jointfile >> rootJoint;
    jointfile.close();
    int HIP_X1 = rootJoint[partname]["x1"].asInt();
    int HIP_Y1 = rootJoint[partname]["y1"].asInt();
    int HIP_BOTTOM_Z = rootJoint[partname]["z1"].asInt();
    int HIP_X2 = rootJoint[partname]["x2"].asInt();
    int HIP_Y2 = rootJoint[partname]["y2"].asInt();
    int HIP_TOP_Z    = rootJoint[partname]["z2"].asInt();
    int HIP_D_Z    = rootJoint[partname]["dz"].asInt();
    int indexLegRP    = rootJoint[partname]["rot1"].asInt();

    Json::Value rootRot;
    std::ifstream rotfile("data/rotPoints.json", std::ifstream::binary);
    rotfile >> rootRot;
    rotfile.close();
    Json::Value const & points = rootRot;
    int ROT_LEG_X  = points[indexLegRP]["xyz"][0].asInt();
    int ROT_LEG_Y  = points[indexLegRP]["xyz"][1].asInt();
    int ROT_LEG_Z  = points[indexLegRP]["xyz"][2].asInt();

    const float dx = 0, dy =150, dz=50;
    const float xmax = 300;
    const float ymax = 370;
    const float zmax = 230;
    Hip hip = {
        {(dx+ROT_LEG_X-HIP_X1)*VOX_X,(ROT_LEG_Y-HIP_Y1+dy)*VOX_Y,(ROT_LEG_Z-HIP_BOTTOM_Z+dz)*VOX_Z},
        {dx*VOX_X, dy*VOX_Y, (HIP_TOP_Z-HIP_BOTTOM_Z+dz)*VOX_Z},
        (HIP_X2-HIP_X1)*VOX_X,
        (HIP_Y2-HIP_Y1)*VOX_Y,
        (HIP_TOP_Z-HIP_BOTTOM_Z)*VOX_Z,
        HIP_D_Z*VOX_Z,
        theta

    };
    BoxNet b = b1.cut( {HIP_X1, HIP_Y1, HIP_BOTTOM_Z}, {HIP_X2, HIP_Y2, HIP_TOP_Z} );
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
                Point3D <float> end = { (ix+0.5)*VOX_X, (iy+0.5)*VOX_Y, (iz+0.5)*VOX_Z };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                hip.getStartPoint( pend, pstart, 3 );
                float x = pstart->x();
                float y = pstart->y();
                float z = pstart->z();
                if (x > 0 && int(std::round(x/VOX_X)) < xmax && y > 0 && int(std::round(y/VOX_Y)) < ymax && z > 0 && int(std::round(z/VOX_Z)) < zmax)
                {
                    unsigned char color = b.getByXyz(int(std::round(x/VOX_X)),int(std::round(y/VOX_Y)),int(std::round(z/VOX_Z)));
                    if (ARM_COLORS.find(color) == ARM_COLORS.end())
                        b2.setByXyz(ix,iy,iz,color);
                    else
                        b2.setByXyz(ix,iy,iz,0);
                }
                else {
                    b2.setByXyz(ix, iy, iz, 0);
                }
            }
        }
    }

    *coord = {HIP_X1-dx, HIP_Y1-dy, HIP_BOTTOM_Z - dz};
//    b2.setPosition(*coord);
    b2.shiftPos(*coord);
    return b2;
}

BoxNet RightHip(BoxNet b1, float theta, Point3D <int> * coord)
{
    return GetHip(b1, theta, coord, true);
}
