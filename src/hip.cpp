#include "hip.h"
#include "json/json.h"
#include <set>

const std::set<unsigned char> ARM_COLORS = {
    11, 19, 20, 21, 22, 23, 59, 104, 108, 118, 124
};

Hip::Hip(Point3D <float> rot, Point3D <float> start, float x, float y, float z, float dz, float theta):
    Joint(
            { start, {start.x(), start.y()+y, start.z()}, {start.x()+x, start.y(), start.z()+dz} },
            { {start.x(), start.y(), start.z()-z}, {start.x()+x, start.y(), start.z()-z}, {start.x(), start.y()+y, start.z()-z} },
            { start, {start.x(), start.y()+y, start.z()}, {start.x()+x, start.y(), start.z()+dz} },
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

/*!
 * \brief startCoefs - ищет начальное приближение для (альфа, бета, t)
 * \param point
 * \return - начальные (альфа, бета, t)
 */
BezierCoords3D * Hip::startCoefs(Point3D<float> * point)
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
                bool hasDef = hip.getStartPoint( pend, pstart, 3 );
                float x = pstart->x();
                float y = pstart->y();
                float z = pstart->z();
                unsigned char color = b.getByXyz(int(x/VOX_X),int(y/VOX_Y),int(z/VOX_Z));
                if (hasDef && x > 0 && x < xmax*VOX_X && y > 0 && y < ymax*VOX_Y && z > 0 && z < zmax*VOX_Z &&
                        ARM_COLORS.find(color) == ARM_COLORS.end())
                {
                    b2.setByXyz(ix,iy,iz,color);
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
