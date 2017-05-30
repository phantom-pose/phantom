#include "slice.h"

/*!
 * \brief Конструктор по умолчанию.
 */

Slice::Slice()
{}

/*!
 * \brief Конструктор со стандартными пропорциями 1:1
 * \param sizeX Размер по оси X
 * \param sizeY Размер по оси Y
 */

Slice::Slice(int sizeX, int sizeY)
    : m_xSize(sizeX), m_ySize(sizeY)
{
    allocateMemory();
}

/*!
 * \brief Конструктор с задаваемыми пропорциями propX:propY
 * \param sizeX Размер по оси X
 * \param sizeY Размер по оси Y
 * \param propX Пропорции по оси X
 * \param propY Пропорции по оси Y
 * \todo Возможная переделка пропорций под float
 */

Slice::Slice(int sizeX, int sizeY, int propX, int propY)
    : m_xSize(sizeX), m_ySize(sizeY)
{
    (propY > propX) ? m_yProp = (float)propY/propX : m_xProp = (float)propX/propY;
    allocateMemory();
}

/*!
 * \brief Конструктор, использующий семантику перемещения (move semantics)
 * \param obj Объект Slice, который будет перемещён
 */

Slice::Slice(Slice && obj)
    : m_data(obj.m_data), m_xSize(obj.m_xSize), m_ySize(obj.m_ySize),
      m_xProp(obj.m_xProp), m_yProp(obj.m_yProp)
{
    // Release the data pointer from the source object so that
    // the destructor does not free the memory multiple times.
    obj.m_data = nullptr;
    obj.m_xSize = 0;
    obj.m_ySize = 0;
    obj.m_xProp = 0;
    obj.m_yProp = 0;
//    Logger::Instance() << "inside move constructor \n";
}

/*!
 * \brief Оператор перемещения (move semantics)
 * \param obj Объект Slice, который будет перемещён
 * \return Slice, которому будут переданы права на перемещаемый объект
 */

Slice & Slice::operator=(Slice && obj)
{
    if (this != &obj)
    {
        // Free the existing resource.
        if (m_data != nullptr) {
            for (int iy = 0; iy < m_ySize; iy++) {
                delete[] m_data[iy];
            }
            delete[] m_data;
        }
        // Copy the data pointer from the source object.
        m_data = obj.m_data;

        m_xSize = obj.m_xSize;
        m_ySize = obj.m_ySize;
        m_xProp = obj.m_xProp;
        m_yProp = obj.m_yProp;

        // Release the data pointer from the source object so that
        // the destructor does not free the memory multiple times.
        obj.m_data = nullptr;
        obj.m_xSize = 0;
        obj.m_ySize = 0;
        obj.m_xProp = 0;
        obj.m_yProp = 0;
    }
//    Logger::Instance() << "inside move assignment \n";
    return *this;
}

/*!
 * \brief Деструктор. Освобождает память, выделенную под unsigned char ** m_data
 */

Slice::~Slice()
{
    if (m_data != nullptr) {
        for (int iy = 0; iy < m_ySize; iy++) {
            delete[] m_data[iy];
        }
        delete[] m_data;
    }
}

/*!
 * \brief Выделение памяти под unsigned char ** m_data
 */

void Slice::allocateMemory()
{
    m_data = new unsigned char *[m_ySize];
    for (int i = 0; i < m_ySize; i++) {
        m_data[i] = new unsigned char[m_xSize];
    }
}

/*!
 * \brief Метод возвращает пропорции по оси Х
 * \return float значение пропорции
 */

float Slice::getPropX() { return m_xProp; }

/*!
 * \brief Метод возвращает пропорции по оси Y
 * \return float значение пропорции
 */

float Slice::getPropY() { return m_yProp; }

/*!
 * \brief Метод возвращает размер по оси X
 * \return int значение размера по оси Х
 */

int Slice::getSizeX() { return m_xSize; }

/*!
 * \brief Метод возвращает размер по оси Y
 * \return int значение размера по оси Y
 */

int Slice::getSizeY() { return m_ySize; }

/*!
 * \brief Метод возвращает значение элемента в положении x, y
 * \param x Координата X
 * \param y Координата Y
 * \return unsigned char значение элемента
 */

unsigned char Slice::getValue(int x, int y) { return m_data[y][x]; }

/*!
 * \brief Метод устанавливает значение value элемента в положении x, y
 * \param x Координата Х
 * \param y Координата Y
 * \param value Значение для помещения в ячейку
 */

void Slice::setValue(int x, int y, unsigned char value) { m_data[y][x] = value; }
