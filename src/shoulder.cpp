#include "shoulder.h"
#include "json/json.h"
#include <set>

const std::set<unsigned char> ARM_COLORS = {
    11, 13, 14, 15, 19, 20, 21, 22, 23, 59, 104, 108, 118, 124
};

Shoulder::Shoulder(Point3D <float> rot, Point3D <float> p1, float dy1, float dz1, Point3D <float> p2, float dx2, float dy2, float phi, float theta):
    Joint(
            { p1, {p1.x(), p1.y(), p1.z()+dz1}, {p1.x(), p1.y()+dy1, p1.z()} },
            { p2, {p2.x()+dx2, p2.y(), p2.z()}, {p2.x(), p2.y()+dy2, p2.z()} },
            { p1, {p1.x(), p1.y(), p1.z()+dz1}, {p1.x(), p1.y()+dy1, p1.z()} },{
                RotateY(rot, RotateX(rot, p2, phi), theta),
                RotateY(rot, RotateX(rot, {p2.x()+dx2, p2.y(), p2.z()}, phi), theta),
                RotateY(rot, RotateX(rot, {p2.x(), p2.y()+dy2, p2.z()}, phi), theta)
            }
        )
{}

BezierCoords3D * Shoulder::choose(float a1, float a2, float b1, float, float l1, float l2)
{
    return new BezierCoords3D(fabs(l1) > fabs(l2) ? a2 : a1, b1, l1/(l1+l2));
}

BoxNet GetShoulder(BoxNet b1, float phi, float theta, Point3D <int> * coord, bool right) {

    std::string partname = right ? "rightShoulder" : "leftShoulder";

    Json::Value rootJoint;
    std::ifstream jointfile("data/jointParams.json", std::ifstream::binary);
    jointfile >> rootJoint;
    jointfile.close();
    int SHOULDER_X1 = rootJoint[partname]["x1"].asInt();
    int SHOULDER_Y = rootJoint[partname]["y"].asInt();
    int SHOULDER_Z1 = rootJoint[partname]["z1"].asInt();
    int SHOULDER_X2 = rootJoint[partname]["x2"].asInt();
    int SHOULDER_Z2 = rootJoint[partname]["z2"].asInt();
    int SHOULDER_DY = rootJoint[partname]["dy"].asInt();
    int SHOULDER_DZ1 = rootJoint[partname]["dz1"].asInt();
    int SHOULDER_DX2 = rootJoint[partname]["dx2"].asInt();
    int indexHandRP    = rootJoint[partname]["rot1"].asInt();

    Json::Value rootRot;
    std::ifstream rotfile("data/rotPoints.json", std::ifstream::binary);
    rotfile >> rootRot;
    rotfile.close();
    Json::Value const & points = rootRot;
    int ROT_HAND_X  = points[indexHandRP]["xyz"][0].asInt();
    int ROT_HAND_Y  = points[indexHandRP]["xyz"][1].asInt();
    int ROT_HAND_Z  = points[indexHandRP]["xyz"][2].asInt();

    const int dx = 70, dy = 70, dz=30;
    const int xmax = dx+(right? SHOULDER_X1 : SHOULDER_X2);
    const int ymax = SHOULDER_DY+140;
    const int zmax = 220;
    Shoulder shoulder = {
        {(dx+ROT_HAND_X-(right? SHOULDER_X2 : SHOULDER_X1))*VOX_X,(ROT_HAND_Y-SHOULDER_Y+dy)*VOX_Y,(ROT_HAND_Z-SHOULDER_Z2+dz)*VOX_Z},
        {(dx+(right? SHOULDER_X1 : 0))*VOX_X, dy*VOX_Y, (SHOULDER_Z1-SHOULDER_Z2+dz)*VOX_Z},
        SHOULDER_DY,
        SHOULDER_DZ1,
        {(dx+(right? 0 : SHOULDER_X2))*VOX_X, dy*VOX_Y, dz*VOX_Z},
        SHOULDER_DX2,
        SHOULDER_DY,
        phi,
        theta

    };
    BoxNet b = b1.cut( {right? SHOULDER_X2 : SHOULDER_X1, SHOULDER_Y, SHOULDER_Z2}, {right? SHOULDER_X1 : SHOULDER_X2, SHOULDER_Y+SHOULDER_DY, SHOULDER_Z1+SHOULDER_DZ1} );
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
                Point3D <float> end = { (ix+0.5f)*VOX_X, (iy+0.5f)*VOX_Y, (iz+0.5f)*VOX_Z };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                shoulder.getStartPoint( pend, pstart, 3 );
                float x = pstart->x();
                float y = pstart->y();
                float z = pstart->z();
                //std::cout << x << " " << y << " " << z << std::endl;
                if (x > 0 && int(std::round(x/VOX_X)) < xmax && y > 0 && int(std::round(y/VOX_Y)) < ymax && z > 0 && int(std::round(z/VOX_Z)) < zmax)
                {
                    unsigned char color = b.getByXyz(int(std::round(x/VOX_X)),int(std::round(y/VOX_Y)),int(std::round(z/VOX_Z)));
                    if (ARM_COLORS.find(color) != ARM_COLORS.end() || int(std::round(z/VOX_Z)) >= dz+(SHOULDER_Z1-SHOULDER_Z2))
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

    *coord = {right? SHOULDER_X2 : SHOULDER_X1, SHOULDER_Y, SHOULDER_Z2};
//    b2.setPosition(*coord);
    b2.shiftPos(*coord);
    return b2;
}


BoxNet RightShoulder(BoxNet b1, float phi, float theta, Point3D <int> * coord)
{
    return GetShoulder(b1, phi, theta, coord, true);
}
