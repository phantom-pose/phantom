#include "phantom.h"
#include "logger.h"

/*!
 * \brief Конструктор без параметров
 * \details Создаёт объект класса Phantom с данными из файла AF_bin, раздроблённый в 2 раза по оси Z.
 * Параметры по размерам ячеек: 1.875 x 1.875 x 2.5 мм..
 */

Phantom::Phantom()
{
    m_boxNet = { 299, 137, 348 };
    m_boxNet.fillFromBin( "data/AF_bin.dat" );
    m_boxNet.segmentation();
}

/*!
 * \brief Конструктор принимающий сетку
 * \param box Сетка, которая копируется в m_boxNet поле
 */

Phantom::Phantom( BoxNet const & box )
{
    m_boxNet = { box };
//    Logger::Instance() << m_boxNet;
}

/*!
 * \brief Метод возвращает значение ячейки по порядковому номеру
 * \param num Порядковый номер ячейки
 * \return unsigned char элемент - id органа
 */

unsigned char Phantom::getValue(int num)
{
    m_boxNet.getByNum(num);
}

/*!
 * \brief Метод возвращает значение ячейки по её координатам x y z в сетке
 * \param x Координата X
 * \param y Координата Y
 * \param z Координата Z
 * \return unsigned char элемент - id органа
 */

unsigned char Phantom::getValue(int x, int y, int z)
{
    m_boxNet.getByXyz(x, y, z);
}

/*!
 * \brief Метод позволяет установить значение ячейки по порядковому номеру num
 * \param num Порядковый номер ячейки
 * \param value Значение unsigned char для помещения в ячейку
 */

void Phantom::setValue(int num, unsigned char value)
{
    m_boxNet.setByNum(num, value);
}

/*!
 * \brief Метод позволяет установить значение ячейки по её координатам x y z в сетке
 * \param x Координата X
 * \param y Координата Y
 * \param z Координата Z
 * \param value Значение unsigned char для помещения в ячейку
 */

void Phantom::setValue(int x, int y, int z, unsigned char value)
{
    m_boxNet.setByXyz(x, y, z, value);
}

/*!
 * \brief Метод возвращает значение Slice для среза по оси X
 * \param num Положение среза вдоль оси Х
 * \return Slice элемент
 */

Slice Phantom::getSliceX(int num) const
{
    Slice slice = m_boxNet.getSliceX(num);
    slice.setProps(m_yScale, m_zScale);
    return slice;
}

/*!
 * \brief Метод возвращает значение Slice для среза по оси Y
 * \param num Положение среза вдоль оси Y
 * \return Slice элемент
 */

Slice Phantom::getSliceY(int num) const
{
    Slice slice = m_boxNet.getSliceY(num);
    slice.setProps(m_xScale, m_zScale);
    return slice;
}

/*!
 * \brief Метод возвращает значение Slice для среза по оси Z
 * \param num Положение среза вдоль оси Z
 * \return Slice элемент
 */

Slice Phantom::getSliceZ(int num) const
{
    Slice slice = m_boxNet.getSliceZ(num);
    slice.setProps(m_xScale, m_yScale);
    return slice;
}

BoxNet const & Phantom::boxNet() const { return m_boxNet; }

int const & Phantom::boxSizeX() const { return m_boxNet.getSizeX(); }
int const & Phantom::boxSizeY() const { return m_boxNet.getSizeY(); }
int const & Phantom::boxSizeZ() const { return m_boxNet.getSizeZ(); }

float const & Phantom::xScale() const { return m_xScale; }
float const & Phantom::yScale() const { return m_yScale; }
float const & Phantom::zScale() const { return m_zScale; }

std::ostream & operator << (std::ostream & os, Phantom const & obj)
{
    os << "|Phantom|\n\
        address = " << &obj << "\n\
        boxAddress = " << &obj.boxNet() << "\n";
    return os;
}
