#include "boundingbox.h"

BoundingBox::BoundingBox()
{

}

BoundingBox::BoundingBox(float xPos, float yPos, float zPos, float a, float b, float c)
    : m_pos(xPos, yPos, zPos), m_a(a), m_b(b), m_c(c)
{}

BoundingBox::BoundingBox(Point3D <float> const & pos, Point3D <float> const & ex, Point3D <float> const & ey, Point3D <float> const & ez, float a, float b, float c)
    : m_pos(pos), m_ex(ex), m_ey(ey), m_ez(ez), m_a(a), m_b(b), m_c(c)
{}

//BoundingBox &  BoundingBox::operator = ( BoundingBox const & obj )
//{
//    m_a    = obj.m_a;
//    m_b    = obj.m_b;
//    m_c    = obj.m_c;
//    m_pos  = obj.m_pos;
//    m_ex   = obj.m_ex;
//    m_ey   = obj.m_ey;
//    m_ez   = obj.m_ez;
//    m_xDir = obj.m_xDir;
//    m_yDir = obj.m_yDir;
//    m_zDir = obj.m_zDir;
//    m_x0   = obj.m_x0;
//    m_y0   = obj.m_y0;
//    m_z0   = obj.m_z0;
//    return *this;
//}

void BoundingBox::setPosition(Point3D<float> const & pos)
{
    m_pos = pos;
}

Point3D<float> const & BoundingBox::getPosition() const
{
    return m_pos;
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
    bool iTrigger = false;
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
    m_xDir = c11 * lineDir.x() + c12 * lineDir.y() + c13 * lineDir.z();
    m_yDir = c21 * lineDir.x() + c22 * lineDir.y() + c23 * lineDir.z();
    m_zDir = c31 * lineDir.x() + c32 * lineDir.y() + c33 * lineDir.z();

    float t1, t2;
    int err = 1;
    // Находим пересечение с плоскостями, перпендикулярными х
    if (fabs(m_xDir) > kEps) {
        // std::cout << "++++HERE++++\n";
        t1 = -m_x0 / m_xDir;
        t2 = (m_a - m_x0) / m_xDir;
//        std::cout << "t1 = " << t1 << " t2 = " << t2 << "\n";
//        std::cout << "Новые координаты:\n";
//        std::cout << "x0 = " << m_x0 << " y0 = " << m_y0 << " z0 = " << m_z0 << "\n";
//        std::cout << "xDir = " << m_xDir << " yDir = " << m_yDir << " zDir = " << m_zDir << "\n";
        // Валидация
        err = validate(t1, 0);
        if (!err) {
            iTrigger = true;
            if (t1 < tmin) {
                tmin = t1;
            }
            if (t1 > tmax) {
                tmax = t1;
            }
        }
        err = validate(t2, 0);
        if (!err) {
            iTrigger = true;
            if (t2 < tmin) {
                tmin = t2;
            }
            if (t2 > tmax) {
                tmax = t2;
            }
        }
    }
    // Находим пересечение с плоскостями, перпендикулярными y
    if (fabs(m_yDir) > kEps) {
        t1 = -m_y0 / m_yDir;
        t2 = (m_b - m_y0) / m_yDir;
        // Валидация
        err = validate(t1, 1);
        if (!err) {
            iTrigger = true;
            if (t1 < tmin) {
                tmin = t1;
            }
            if (t1 > tmax) {
                tmax = t1;
            }
        }
        err = validate(t2, 1);
        if (!err) {
            iTrigger = true;
            if (t2 < tmin) {
                tmin = t2;
            }
            if (t2 > tmax) {
                tmax = t2;
            }
        }
    }
    // Находим пересечение с плоскостями, перпендикулярными z
    if (fabs(m_zDir) > kEps) {
        t1 = -m_z0 / m_zDir;
        t2 = (m_c - m_z0) / m_zDir;
        // Валидация
        err = validate(t1, 2);
        if (!err) {
            iTrigger = true;
            if (t1 < tmin) {
                tmin = t1;
            }
            if (t1 > tmax) {
                tmax = t1;
            }
        }
        err = validate(t2, 2);
        if (!err) {
            iTrigger = true;
            if (t2 < tmin) {
                tmin = t2;
            }
            if (t2 > tmax) {
                tmax = t2;
            }
        }
    }
    if (iTrigger) { return 0; }
    return 1;
}

int BoundingBox::intersect(Line const & line, Segment & s)
{
    int res = intersect(line, s.pos, s.end);
    return res;
}

/*!
 * \brief validate Валидация параметра t, пересекающего плоскость
 * \param t
 * \param pFactor номер плоскости пересечение с которой валидируется 0-x, 1-y, 2-z
 * \return
 */

int BoundingBox::validate(float const & t, int pFactor)
{
    if (t > 0) {
        if (pFactor == 0) {
            float y = m_y0 + m_yDir * t;
            float z = m_z0 + m_zDir * t;
            if ( y > 0 && y < m_b && z > 0 && z < m_c ) {
                return 0;
            } else {
                return 1;
            }
        } else if (pFactor == 1) {
            float x = m_x0 + m_xDir * t;
            float z = m_z0 + m_zDir * t;
            if ( x > 0 && x < m_a && z > 0 && z < m_c ) {
                return 0;
            } else {
                return 1;
            }
        } else if (pFactor == 2) {
            float x = m_x0 + m_xDir * t;
            float y = m_y0 + m_yDir * t;
            if ( x > 0 && x < m_a && y > 0 && y < m_b ) {
                return 0;
            } else {
                return 1;
            }
        }
    }
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
//    return m_pos;
}

std::ostream & operator << (std::ostream & os, BoundingBox const & obj)
{
    os << "|BoundingBox|\n\
        position  = {" << obj.getPosition().getShrink(1.775, 1.775, 4.84) << "}\n\
        end = {" << obj.getEndpoint().getShrink(1.775, 1.775, 4.84) << "}\n";
//        ex = {" << obj.getEx() << "}\n\
//        ey = {" << obj.getEy() << "}\n\
//        ez = {" << obj.getEz() << "}\n
    return os;
}
