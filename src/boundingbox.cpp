#include "boundingbox.h"

BoundingBox::BoundingBox(float xPos, float yPos, float zPos, float a, float b, float c)
    : m_pos(xPos, yPos, zPos), m_a(a), m_b(b), m_c(c)
{}

BoundingBox::BoundingBox(float xPos, float yPos, float zPos, float a, float b, float c, std::string name)
    : m_pos(xPos, yPos, zPos), m_a(a), m_b(b), m_c(c), m_name(name)
{}

BoundingBox::BoundingBox(Point3D <float> const & pos, Point3D <float> const & ex, Point3D <float> const & ey, Point3D <float> const & ez, float a, float b, float c)
    : m_pos(pos), m_ex(ex), m_ey(ey), m_ez(ez), m_a(a), m_b(b), m_c(c)
{}

void BoundingBox::setPosition(Point3D<float> const & pos)
{
    m_pos = pos;
}

Point3D<float> const & BoundingBox::getPosition() const
{
    return m_pos;
}

float const & BoundingBox::a() const
{
    return m_a;
}

float const & BoundingBox::b() const
{
    return m_b;
}

float const & BoundingBox::c() const
{
    return m_c;
}

std::string const & BoundingBox::getName() const
{
    return m_name;
}

void BoundingBox::rotate(RotationMatrix matrix)
{
    m_ex = m_ex + m_pos;
    m_ey = m_ey + m_pos;
    m_ez = m_ez + m_pos;

    matrix.Rotate(m_pos);
    matrix.Rotate(m_ex);
    matrix.Rotate(m_ey);
    matrix.Rotate(m_ez);

    m_ex = m_ex - m_pos;
    m_ey = m_ey - m_pos;
    m_ez = m_ez - m_pos;
}

int BoundingBox::intersect(Line const & line, float & tmin, float & tmax)
{
//    std::cout << "intersect " << m_name << std::endl;
    float values[2];
    values[0] = 0;
    values[1] = 0.0f;
    int   valuesCounter = 0;
    Point3D<float> linePos = line.getPosition();
    Point3D<float> lineDir = line.getDirection();
    // Проводим расчёты в с.к. п-пипеда
    // Позиция линии относительно позиции п-пипеда
    Point3D<float> _linePos = linePos - m_pos;
    // -- Поворачиваем позицию --
    // Формируем матрицу перехода от одного базиса к другому
    float c11 = m_ex.x(), c12 = m_ey.x(), c13 = m_ez.x();
    float c21 = m_ex.y(), c22 = m_ey.y(), c23 = m_ez.y();
    float c31 = m_ex.z(), c32 = m_ey.z(), c33 = m_ez.z();

    // Находим новые координаты
    m_x0 = c11 * _linePos.x() + c21 * _linePos.y() + c31 * _linePos.z();
    m_y0 = c12 * _linePos.x() + c22 * _linePos.y() + c32 * _linePos.z();
    m_z0 = c13 * _linePos.x() + c23 * _linePos.y() + c33 * _linePos.z();

    // -- Поворачиваем направление --
    m_xDir = c11 * lineDir.x() + c21 * lineDir.y() + c31 * lineDir.z();
    m_yDir = c12 * lineDir.x() + c22 * lineDir.y() + c32 * lineDir.z();
    m_zDir = c13 * lineDir.x() + c23 * lineDir.y() + c33 * lineDir.z();

    float t1, t2;
    int err = 1;
    // Находим пересечение с плоскостями, перпендикулярными х
    if (fabs(m_xDir) > kEps) {
//        std::cout << "INTERSECT WITH X" << std::endl;
        t1 = -m_x0 / m_xDir;
        t2 = (m_a - m_x0) / m_xDir;
//        std::cout << "t1 = " << t1 << " t2 = " << t2 << "\n";
//        std::cout << "Новые координаты:\n";
//        std::cout << "x0 = " << m_x0 << " y0 = " << m_y0 << " z0 = " << m_z0 << "\n";
//        std::cout << "xDir = " << m_xDir << " yDir = " << m_yDir << " zDir = " << m_zDir << "\n";
        // Валидация
        err = validate(t1, 0);
        if (!err) {
//            std::cout << "SUCCESS VALIDATE t1 = " << t1 << std::endl;
            values[valuesCounter++] = t1;
        }
        err = validate(t2, 0);
        if (!err) {
//            std::cout << "SUCCESS VALIDATE t2 = " << t2 << std::endl;
            values[valuesCounter++] = t2;
        }
    }
    // Находим пересечение с плоскостями, перпендикулярными y
    if (fabs(m_yDir) > kEps) {
//        std::cout << "INTERSECT WITH Y" << std::endl;
        t1 = -m_y0 / m_yDir;
        t2 = (m_b - m_y0) / m_yDir;
        // Валидация
        err = validate(t1, 1);
        if (!err) {
//            std::cout << "SUCCESS VALIDATE t1 = " << t1 << std::endl;
            values[valuesCounter++] = t1;
        }
        err = validate(t2, 1);
        if (!err) {
//            std::cout << "SUCCESS VALIDATE t2 = " << t2 << std::endl;
            values[valuesCounter++] = t2;
        }
    }
    // Находим пересечение с плоскостями, перпендикулярными z
    if (fabs(m_zDir) > kEps) {
//        std::cout << "INTERSECT WITH Z" << std::endl;
        t1 = -m_z0 / m_zDir;
        t2 = (m_c - m_z0) / m_zDir;
//        std::cout << "z0 = " << m_z0 << " zDir = " << m_zDir << " c = " << m_c << "\n";
//        usleep(10000);
//        std::cout << "t1 = " << t1 << std::endl;
        // Валидация
        err = validate(t1, 2);
        if (!err) {
//            std::cout << "SUCCESS VALIDATE t1 = " << t1 << std::endl;
            values[valuesCounter++] = t1;
        }
        err = validate(t2, 2);
        if (!err) {
//            std::cout << "SUCCESS VALIDATE t2 = " << t2 << std::endl;
            values[valuesCounter++] = t2;
        }
    }

    if (!valuesCounter) {
        return 1; // Нет пересечений
    } else {
        if (values[0] > values[1]) {
            tmax = values[0];
            tmin = values[1];
        } else {
            tmax = values[1];
            tmin = values[0];
        }
        return 0;
    }
    return 2;
}

int BoundingBox::intersect(Line const & line, Segment & s)
{
    int res = intersect(line, s.pos, s.end);
    return res;
}

bool BoundingBox::hasInsideBox(float x, float y, float z)
{
    return (x > 0 && y > 0 && z > 0 && x < m_a && y < m_b && z < m_c);
}

/*!
 * \brief validate Валидация параметра t, пересекающего плоскость
 * \param t
 * \param pFactor номер плоскости пересечение с которой валидируется 0-x, 1-y, 2-z
 * \return
 */

int BoundingBox::validate(float const & t, int pFactor)
{
    float x, y, z;
//    std::cout << "t = " << t << std::endl;
    if (t > 0) {
        if (pFactor == 0) {
            y = m_y0 + m_yDir * t;
            z = m_z0 + m_zDir * t;
            if ( y > 0 && y < m_b && z > 0 && z < m_c ) {
                return 0;
            } else {
                return 1;
            }
        } else if (pFactor == 1) {
            x = m_x0 + m_xDir * t;
            z = m_z0 + m_zDir * t;
            if ( x > 0 && x < m_a && z > 0 && z < m_c ) {
                return 0;
            } else {
                return 1;
            }
        } else if (pFactor == 2) {
//            std::cout << "validate t = " << t << std::endl;
            x = m_x0 + m_xDir * t;
            y = m_y0 + m_yDir * t;
            if ( x > 0 && x < m_a && y > 0 && y < m_b ) {
                return 0;
            } else {
                return 1;
            }
        }
    }
    return 1;
}

Point3D<float> const & BoundingBox::getEx() const { return m_ex; }
Point3D<float> const & BoundingBox::getEy() const { return m_ey; }
Point3D<float> const & BoundingBox::getEz() const { return m_ez; }

Point3D<float> const & BoundingBox::getPos(float h) const
{
    return m_pos + m_ez * h;
}

Point3D<float> const & BoundingBox::getEx(float h) const
{
    return m_pos + m_ez * h + m_ex * m_a;
}

Point3D<float> const & BoundingBox::getEy(float h) const
{
    return m_pos + m_ez * h + m_ey * m_b;
}

Point3D<float> BoundingBox::getEndpoint() const
{
    Point3D <float> a = m_ex * m_a;
    Point3D <float> b = m_ey * m_b;
    Point3D <float> c = m_ez * m_c;
    return m_pos + a + b + c;
}

void BoundingBox::Serialize(Json::Value & root)
{
    m_pos.Serialize(root["pos"]);
    m_ex.Serialize(root["ex"]);
    m_ey.Serialize(root["ey"]);
    m_ez.Serialize(root["ez"]);
    root["a"] = m_a;
    root["b"] = m_b;
    root["c"] = m_c;
    root["name"] = m_name;
}

void BoundingBox::Deserialize(Json::Value & root)
{
    m_pos.Deserialize( root["pos"] );
    m_ex.Deserialize( root["ex"] );
    m_ey.Deserialize( root["ey"] );
    m_ez.Deserialize( root["ez"] );
    m_a = root.get("a", 0).asFloat();
    m_b = root.get("b", 0).asFloat();
    m_c = root.get("c", 0).asFloat();
    m_name = root.get("name", 0).asString();
}

std::ostream & operator << (std::ostream & os, BoundingBox const & obj)
{
    os << "|BoundingBox|\n\
        name  = {" << obj.getName() << "}\n\
        position  = {" << obj.getPosition() << "}\n\
        ex = {" << obj.getEx() << "}\n\
        ey = {" << obj.getEy() << "}\n\
        ez = {" << obj.getEz() << "}\n\
        a = {" << obj.a() << "}\n\
        b = {" << obj.b() << "}\n\
        c = {" << obj.c() << "}\n";
    return os;
}
