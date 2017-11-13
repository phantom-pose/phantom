#pragma once

#include "point3d.hpp"
#include "rotationmatrix.h"
#include "line.h"
#include "ijsonserializable.h"
#include "json/json.h"

class BoundingBox : public IJsonSerializable
{
public:
    BoundingBox() = default;
    ~BoundingBox() override = default;
    BoundingBox(float xPos, float yPos, float zPos, float a, float b, float c);
    BoundingBox(float xPos, float yPos, float zPos, float a, float b, float c, std::string name);
    BoundingBox(Point3D <float> const & pos, Point3D <float> const & ex, Point3D <float> const & ey, Point3D <float> const & ez, float a, float b, float c);
    void setPosition(Point3D<float> const & pos);

    bool hasInsideBox(float x, float y, float z);
//    BoundingBox &  operator = ( BoundingBox const & obj );
    Point3D<float> const & getPosition() const;
    Point3D<float> const & getEx() const;
    Point3D<float> const & getEy() const;
    Point3D<float> const & getEz() const;
    float const & a() const;
    float const & b() const;
    float const & c() const;
    std::string const & getName() const;
    Point3D<float> const & getPos(float h) const;
    Point3D<float> const & getEx(float h) const;
    Point3D<float> const & getEy(float h) const;
    void rotate(RotationMatrix matrix);
    int intersect(Line const & line, float & tmin, float & tmax);
    int intersect(Line const & line, Segment & s);
    int validate(float const &t, int pFactor);
    Point3D<float> getEndpoint() const;
    void Serialize( Json::Value & root );
    void Deserialize( Json::Value & root );


private:
    std::string m_name;
    float m_a, m_b, m_c;
    Point3D<float> m_pos = { 0, 0, 0 };
    Point3D<float> m_ex = { 1, 0, 0 };
    Point3D<float> m_ey = { 0, 1, 0 };
    Point3D<float> m_ez = { 0, 0, 1 };
    // Относится к Line
    float m_x0, m_y0, m_z0;
    float m_xDir, m_yDir, m_zDir;
};

std::ostream & operator << (std::ostream & os, BoundingBox const & obj);
