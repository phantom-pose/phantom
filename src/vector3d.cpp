#include "vector3d.h"

/*!
 * \brief Vector3D::Vector3D конструктор вектора с началом в точке { 0, 0 }, направлением direction и длиной 1
 */
Vector3D::Vector3D(Point3D <float> direction)
    : m_x( direction.x() ), m_y( direction.y() ), m_z( direction.z() )
{
    normalization();
    calculateEndpoint();
}

/*!
 * \brief Vector3D::Vector3D конструктор вектора с началом в точке { 0, 0 }, направлением { dirX, dirY, dirZ } и длиной 1
 */
Vector3D::Vector3D(float const & dirX, float const & dirY, float const & dirZ)
    : m_x(dirX), m_y(dirY), m_z(dirZ), m_x1(dirX), m_y1(dirY), m_z1(dirZ)
{
    normalization();
    calculateEndpoint();
}

/*!
 * \brief Vector3D::Vector3D конструктор вектора c заданным началом и направлением. длина остается единичной
 */
Vector3D::Vector3D(Point3D <float> position, Point3D <float> direction)
    : m_x( direction.x() ), m_y( direction.y() ), m_z( direction.z() ),
      m_x0( position.x() ), m_y0( position.y() ), m_z0( position.z() )
{
    normalization();
    calculateEndpoint();
}

/*!
 * \brief Vector3D::Vector3D конструктор вектора c заданным началом, направлением и длиной
 */
Vector3D::Vector3D(Point3D <float> position, Point3D <float> direction, float const & len)
    : m_x( direction.x() ), m_y( direction.y() ), m_z( direction.z() ),
      m_x0( position.x() ), m_y0( position.y() ), m_z0( position.z() ), m_len(len)
{
    normalization();
    calculateEndpoint();
}

/*!
 * \brief Vector3D::Vector3D конструктор вектора c заданным началом и концом
 */
Vector3D::Vector3D(Point3D <float> position, Point3D <float> endpoint)
    : m_x0( position.x() ), m_y0( position.y() ), m_z0( position.z() ),
      m_x1( endpoint.x() ), m_y1( endpoint.y() ), m_z1( endpoint.z() )
{
    m_x = m_x1 - m_x0;
    m_y = m_y1 - m_y0;
    m_z = m_z1 - m_z0;
    m_len = sqrt( m_x * m_x + m_y * m_y + m_z * m_z );
    normalization();
}

/*!
 * \brief Vector3D::setPosition устанавливает начало вектора. направление не меняется, меняется конец
 */
void Vector3D::setPosition(float const & x, float const & y, float const & z)
{
    m_x0 = x;
    m_y0 = y;
    m_z0 = z;
    calculateEndpoint();
}

/*!
 * \brief Vector3D::setDirection устанавливает направление вектора, при это меняется конец
 */
void Vector3D::setDirection(float const & x, float const & y, float const & z)
{
    m_x = x;
    m_y = y;
    m_z = z;
    normalization();
    calculateEndpoint();
}

/*!
 * \brief Vector3D::setEndpoint устанавливает конец вектора, при этом меняется направление, но не меняется начало
 */
void Vector3D::setEndpoint(float const & x, float const & y, float const & z)
{
    m_x1 = x;
    m_y1 = y;
    m_z1 = z;

    m_x = m_x1 - m_x0;
    m_y = m_y1 - m_y0;
    m_z = m_z1 - m_z0;
    m_len = sqrt( m_x*m_x + m_y*m_y + m_z*m_z );
    normalization();
}

/*!
 * \brief Vector3D::setLength устанавливаем длину. Не меняется начало и направление. меняется конец
 */
void Vector3D::setLength(float const & len)
{
    m_len = len;
    calculateEndpoint();
}

/*!
 * \brief Vector3D::setPosition устанавливает начало вектора. направление не меняется, меняется конец
 */
void Vector3D::setPosition(Point3D <float> const & position)
{
    setPosition( position.x(), position.y(), position.z() );
}

/*!
 * \brief Vector3D::setDirection устанавливает направление вектора, при это меняется конец
 */
void Vector3D::setDirection(Point3D <float> const & direction)
{
    setDirection( direction.x(), direction.y(), direction.z() );
}

/*!
 * \brief Vector3D::setEndpoint устанавливает конец вектора, при этом меняется направление, но не меняется начало
 */
void Vector3D::setEndpoint(Point3D <float> const & endpoint)
{
    setEndpoint( endpoint.x(), endpoint.y(), endpoint.z() );
}

// Геттеры. обычные.
Point3D <float> Vector3D::getPosition() const
{
    return { m_x0, m_y0, m_z0 };
}
Point3D <float> Vector3D::getDirection() const
{
    return { m_x, m_y, m_z };
}
Point3D <float> Vector3D::getEndpoint() const
{
    return { m_x1, m_y1, m_z1 };
}
float Vector3D::getLength() const
{
    return m_len;
}

/*!
 * \brief Vector3D::operator == проверка 2х векторов на равенство
 * TODO добавить проверку длины с точностью эпсилон
 */
bool Vector3D::operator == (Vector3D const & obj) const
{
    Point3D <float> p1 = { m_x0, m_y0, m_z0 };
    Point3D <float> d1 = { m_x, m_y, m_z };
    Point3D <float> ep1 = { m_x1, m_y1, m_z1 };
    Point3D <float> p2 = { obj.m_x0, obj.m_y0, obj.m_z0 };
    Point3D <float> d2 = { obj.m_x, obj.m_y, obj.m_z };
    Point3D <float> ep2 = { obj.m_x1, obj.m_y1, obj.m_z1 };
    return ( p1 == p2 && d1 == d2 && ep1 == ep2 );
}

/*!
 * \brief Vector3D::operator = Оператор присваивания. Копирует данные одного вектора в другой
 */
Vector3D & Vector3D::operator = ( Vector3D const & obj )
{
    if ( this == &obj ) return *this;
    m_x0 = obj.m_x0;
    m_y0 = obj.m_y0;
    m_z0 = obj.m_z0;

    m_x = obj.m_x;
    m_y = obj.m_y;
    m_z = obj.m_z;

    m_x1 = obj.m_x1;
    m_y1 = obj.m_y1;
    m_z1 = obj.m_z1;

    m_len = obj.m_len;
    return *this;
}

/*!
 * \brief Vector3D::operator * оператор умножения. Перемножает вектора а и b ТОЛЬКО ЕСЛИ У НИХ ОДИНАКОВАЯ НАЧАЛЬНАЯ ТОЧКА
 * иначе возвращает вектор  0 0 0 -> -1 -1 -1, в будущем необходимо бросать исключение
 */
Vector3D Vector3D::operator * (Vector3D const & obj) const
{
    Point3D <float> pos = { m_x0, m_y0, m_z0 };
    Vector3D res = { -1, -1, -1 };
    if (pos != obj.getPosition()) { return res; }
    float ax = m_x1 - m_x0;
    float ay = m_y1 - m_y0;
    float az = m_z1 - m_z0;
    Point3D <float> objPos = obj.getPosition();
    Point3D <float> objEp = obj.getEndpoint();
    float bx = objEp.x() - objPos.x();
    float by = objEp.y() - objPos.y();
    float bz = objEp.z() - objPos.z();

    float epX = ay * bz - az * by;
    float epY = az * bx - ax * bz;
    float epZ = ax * by - ay * bx;
    Point3D <float> ep = { epX, epY, epZ };
    res.setPosition( pos );
    res.setEndpoint( ep + pos );
    return res;
}


/*!
 * \brief Vector3D::calculateEndpoint внутренняя функция расчета
 */
void Vector3D::calculateEndpoint()
{
    m_x1 = m_x0 + m_x * m_len;
    m_y1 = m_y0 + m_y * m_len;
    m_z1 = m_z0 + m_z * m_len;
}

/*!
 * \brief Vector3D::normalization внутренняя функция нормализации направления
 */
void Vector3D::normalization()
{
    float hyp = sqrt( m_x * m_x + m_y * m_y + m_z * m_z );
    m_x /= hyp;
    m_y /= hyp;
    m_z /= hyp;
}
