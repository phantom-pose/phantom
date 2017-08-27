#pragma once

#include <cmath>
#include <functional>
#include <ostream>

float const kEps = 1e-5;

/*!
 * \brief Point3D - шаблонный класс, реализующий работу с координатой { float float float } или
 * положением в сетке { int int int }
 */
template < class T >
class Point3D
{
public:
    /*!
     * \brief Конструктор по умолчанию, устанавливаются нулевые параметры
     */
    Point3D() = default;

    /*!
     * \brief Конструктор копирования
     * \param obj копируемый объект
     */
    Point3D(Point3D const & obj)
        : m_x(obj.m_x), m_y(obj.m_y), m_z(obj.m_z)
    {}

    /*!
     * \brief Конструктор с параметрами
     */
    Point3D(T x, T y, T z)
        : m_x(x), m_y(y), m_z(z)
    {}

    /*!
     * \brief Конструктор перемещения
     */
    Point3D(Point3D && obj)
    {
        std::swap(m_x, obj.m_x);
        std::swap(m_y, obj.m_y);
        std::swap(m_z, obj.m_z);
    }

    /*!
     * \brief Оператор перемещения
     */
    Point3D & operator = (Point3D && obj)
    {
        if (*this == obj) return *this;
        std::swap(m_x, obj.m_x);
        std::swap(m_y, obj.m_y);
        std::swap(m_z, obj.m_z);
    }

    /*!
     * \brief Оператор присваивания
     */
    Point3D & operator = (Point3D const & obj)
    {
        if (*this == obj) return *this;
        m_x = obj.m_x;
        m_y = obj.m_y;
        m_z = obj.m_z;
    }

    /*!
     * \brief геттер x
     */
    T const & x() const { return m_x; }
    /*!
     * \brief геттер y
     */
    T const & y() const { return m_y; }
    /*!
     * \brief геттер z
     */
    T const & z() const { return m_z; }

    /*!
     * \brief Оператор логического равенства для Point3D<int>
     */
    bool operator == (Point3D<int> const & obj) const
    {
        return m_x == obj.m_x && m_y == obj.m_y && m_z == obj.m_z;
    }

    /*!
     * \brief Оператор логического равенства для Point3D<float>
     */
    bool operator == (Point3D<float> const & obj) const
    {
        return EqualWithEps(m_x, obj.m_x) && EqualWithEps(m_y, obj.m_y) && EqualWithEps(m_z, obj.m_z);
    }

    /*!
     * \brief Оператор логического неравенства
     */
    bool operator != (Point3D const & obj) const
    {
        return !operator==(obj);
    }

    /*!
     * \brief Оператор сложения
     */
    Point3D operator + (Point3D const & obj) const
    {
        return { m_x + obj.m_x, m_y + obj.m_y, m_z + obj.m_z };
    }

    /*!
     * \brief Оператор вычитания
     */
    Point3D operator - (Point3D const & obj) const
    {
        return { m_x - obj.m_x, m_y - obj.m_y, m_z - obj.m_z };
    }

    /*!
     * \brief Умножение на число
     */
    Point3D operator * (float scale) const
    {
        return { m_x * scale, m_y * scale, m_z * scale };
    }

    /*!
     * \brief Оператор меньше
     */
    bool operator < (Point3D const & obj) const
    {
        return m_x < obj.m_x && m_y < obj.m_y && m_z < obj.m_z;
    }

    /*!
     * \brief Оператор меньше или равно
     */
    bool operator <= (Point3D const & obj) const
    {
        return m_x <= obj.m_x && m_y <= obj.m_y && m_z <= obj.m_z;
    }

    /*!
     * \brief Оператор больше
     */
    bool operator > (Point3D const & obj) const
    {
        return m_x > obj.m_x && m_y > obj.m_y && m_z > obj.m_z;
    }

    /*!
     * \brief Оператор больше или равно
     */
    bool operator >= (Point3D const & obj) const
    {
        return m_x >= obj.m_x && m_y >= obj.m_y && m_z >= obj.m_z;
    }

    void setValue(T x, T y, T z)
    {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    float operator * (Point3D const & obj) const {
        return m_x * obj.m_x + m_y * obj.m_y + m_z * obj.m_z;
    }

private:
    /*!
     * \brief Служебный метод определения равенства двух float чисел с точностью kEps
     */
    bool EqualWithEps( float f1, float f2 ) const
    {
        return fabs(f1 - f2) < kEps;
    }

    T m_x = T();
    T m_y = T();
    T m_z = T();
};

template< typename T >
std::ostream & operator << (std::ostream & os, Point3D<T> const & obj)
{
    os << "|Point3D|\n\
        x [" << &obj.x() << "] = " << obj.x() << "\n\
        y [" << &obj.y() << "] = " << obj.y() << "\n\
        z [" << &obj.z() << "] = " << obj.z() << "\n";
    return os;
}
