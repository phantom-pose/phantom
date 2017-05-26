#include "slice.h"

// Конструктор со стандартными пропорциями 1:1
Slice::Slice(int sizeX, int sizeY)
    : m_xSize(sizeX), m_ySize(sizeY)
{
    allocateMemory();
}

// Конструктор с задаваемыми пропорциями propX:propY
Slice::Slice(int sizeX, int sizeY, int propX, int propY)
    : m_xSize(sizeX), m_ySize(sizeY)
{
    (propY > propX) ? m_yProp = (float)propY/propX : m_xProp = (float)propX/propY;
    allocateMemory();
}

// Конструктор перемещения
Slice::Slice(Slice && obj)
    : m_data(nullptr)
{
    // Copy the data pointer from the source object.
    m_data = obj.m_data;

    m_xSize = obj.m_xSize;
    m_ySize = obj.m_ySize;
    m_xProp = obj.m_xProp;
    m_yProp = obj.m_yProp;

    // Release the data pointer from the source object so that
    // the destructor does not free the memory multiple times.
    obj.m_data = nullptr;
}

// Оператор перемещения
Slice & Slice::operator=(Slice && obj)
{
    if (this != &obj)
    {
        // Free the existing resource.
        ~Slice();
        // Copy the data pointer from the source object.
        m_data = obj.m_data;

        m_xSize = obj.m_xSize;
        m_ySize = obj.m_ySize;
        m_xProp = obj.m_xProp;
        m_yProp = obj.m_yProp;

        // Release the data pointer from the source object so that
        // the destructor does not free the memory multiple times.
        obj.m_data = nullptr;
    }
    return *this;
}

// Деструктор
Slice::~Slice()
{
    if (m_data != nullptr) {
        for (int iy = 0; iy < m_ySize; iy++) {
            delete[] m_data[iy];
        }
        delete[] m_data;
    }
}

// Allocate memory for data array
void Slice::allocateMemory()
{
    m_data = new unsigned char *[m_ySize];
    for (int i = 0; i < m_ySize; i++) {
        m_data[i] = new unsigned char[m_xSize];
    }
}

float Slice::getPropX() { return m_xProp; }
float Slice::getPropY() { return m_yProp; }

int Slice::getSizeX() { return m_xSize; }
int Slice::getSizeY() { return m_ySize; }

unsigned char Slice::getValue(int x, int y) { return m_data[y][x]; }
void Slice::setValue(int x, int y, unsigned char value) { m_data[y][x] = value; }

//// Оператор присваивания.
//Slice & Slice::operator = (Slice const & obj)
//{
//    if (this == &obj) return *this;
//    m_xSize = obj.m_xSize;
//    m_ySize = obj.m_ySize;
//    m_xProp = obj.m_xProp;
//    m_yProp = obj.m_yProp;

//    for (int iy = 0; iy < obj.getSizeY(); iy++) {
//            for (int ix = 0; ix < m_boxNet.getSizeX(); ix++) {
//                if (slice.getValue(ix, iy) != 0)
//                {
//                    painter.drawRect(X_SCALE * ix, Y_SCALE * iy, X_SCALE, Y_SCALE);
//                }
//            }
//        }
//    return *this;
//}

// Оператор перемещения
//Slice & Slice::operator = (Slice const && obj)
//{
//    ~Slice();

//}
