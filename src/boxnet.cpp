#include "boxnet.h"
#include "logger.h"

/*!
 * \brief Конструктор с тремя параметрами - размерами сетки
 * \param sizeX Размер сетки по оси Х
 * \param sizeY Размер сетки по оси Y
 * \param sizeZ Размер сетки по оси Z
 */

BoxNet::BoxNet(int sizeX, int sizeY, int sizeZ)
    : m_xSize(sizeX), m_ySize(sizeY), m_zSize(sizeZ)
{
    m_length = sizeX * sizeY * sizeZ;
    m_list = new unsigned char[m_length];
    //fill array with zeroes
    for (int i = 0; i < m_length; i++) {
        m_list[i] = 0;
    }
}

/*!
 * \brief Деструктор, освобождает память массива m_list
 */

BoxNet::~BoxNet()
{
    delete[] m_list;
}

/*!
 * \brief Метод возвращает значение ячейки по порядковому номеру
 * \param num Порядковый номер ячейки
 * \return unsigned char элемент - id органа
 */

unsigned char BoxNet::getByNum(int num)
{
    return m_list[num];
}

/*!
 * \brief Метод возвращает значение ячейки по её координатам x y z в сетке
 * \param x Координата X
 * \param y Координата Y
 * \param z Координата Z
 * \return unsigned char элемент - id органа
 */

unsigned char BoxNet::getByXyz(int x, int y, int z)
{
    int num = z * m_xSize * m_ySize + y * m_xSize + x;
    return m_list[num];
}

/*!
 * \brief Метод позволяет установить значение ячейки по порядковому номеру num
 * \param num Порядковый номер ячейки
 * \param value Значение unsigned char для помещения в ячейку
 */

void BoxNet::setByNum(int num, unsigned char value)
{
    m_list[num] = value;
}

/*!
 * \brief Метод позволяет установить значение ячейки по её координатам x y z в сетке
 * \param x Координата X
 * \param y Координата Y
 * \param z Координата Z
 * \param value Значение unsigned char для помещения в ячейку
 */

void BoxNet::setByXyz(int x, int y, int z, unsigned char value)
{
    int num = z * m_xSize * m_ySize + y * m_xSize + x;
    m_list[num] = value;
}

/*!
 * \brief Метод для получения размера сетки по оси X
 * \return Размер сетки по оси X
 */

int BoxNet::getSizeX() { return m_xSize; }

/*!
 * \brief Метод для получения размера сетки по оси Y
 * \return Размер сетки по оси Y
 */

int BoxNet::getSizeY() { return m_ySize; }

/*!
 * \brief Метод для получения размера сетки по оси Z
 * \return Размер сетки по оси Z
 */

int BoxNet::getSizeZ() { return m_zSize; }

/*!
 * \brief Метод возвращает значение Slice для среза по оси X
 * \param num Положение среза вдоль оси Х
 * \return Slice элемент
 */

Slice BoxNet::getSliceX(int num) const
{
    if (num < 0 || num > m_ySize) {
        Slice slice;
        return slice;
    }
    Slice slice = { m_xSize, m_zSize };

    for (int iz = 0; iz < m_zSize; iz++)
    {
        for (int iy = 0; iy < m_xSize; iy++)
        {
            int n = iz * m_xSize * m_ySize + iy * m_xSize + num;
            slice.setValue(iy, iz, m_list[n]);
        }
    }
    return slice;
}

/*!
 * \brief Метод возвращает значение Slice для среза по оси Y
 * \param num Положение среза вдоль оси Y
 * \return Slice элемент
 */

Slice BoxNet::getSliceY(int num) const
{
    if (num < 0 || num > m_ySize) {
        Slice slice;
        return slice;
    }
    Slice slice = { m_xSize, m_zSize };

    for (int iz = 0; iz < m_zSize; iz++)
    {
        for (int ix = 0; ix < m_xSize; ix++)
        {
            int n = iz * m_xSize * m_ySize + num * m_xSize + ix;
            slice.setValue(ix, iz, m_list[n]);
        }
    }
    return slice;
}

/*!
 * \brief Метод возвращает значение Slice для среза по оси Z
 * \param num Положение среза вдоль оси Z
 * \return Slice элемент
 */

Slice BoxNet::getSliceZ(int num) const
{
    if (num < 0 || num > m_zSize) {
        Slice slice;
        return slice;
    }
    Slice slice = { m_xSize, m_ySize };

    for (int iy = 0; iy < m_ySize; iy++)
    {
        for (int ix = 0; ix < m_xSize; ix++)
        {
            int n = num * m_xSize * m_ySize + iy * m_xSize + ix;
            slice.setValue(ix, iy, m_list[n]);
        }
    }
    return slice;
}

/*!
 * \brief Метод заполнения сетки из файла
 * \param filename имя файла, относительный путь
 */

void BoxNet::fillFromFile(char const * filename)
{
    std::ifstream f(filename);
    char line[100];
    int i = 0;
    int ival, n;
    if(f.is_open())
    {
        while (f.getline(line,100))
        {
            char *data = line;
            while (sscanf(data, "%d%n", &ival, &n) == 1)
            {
                data += n;
                m_list[i++] = (unsigned char)ival;
            }
        }
        f.close();
    } else {
        printf("Cant open file\n");
    }
}

/*!
 * \brief Метод записывает содержимое сетки в двоичный файл
 * \param filename имя файла, относительный путь
 */

void BoxNet::writeBinFile(char const * filename)
{
    int i = 0;
    std::ofstream file(filename, std::ios::binary);
    while (i < m_xSize * m_ySize * m_zSize)
    {
        file.write(reinterpret_cast<char const*>(&m_list[i]), sizeof(m_list[i]));
        i++;
    }
    file.close();
}

/*!
 * \brief Метод заполнения сетки из двоичного файла
 * \param filename имя файла, относительный путь
 */

void BoxNet::fillFromBin(char const * filename)
{
    int i = 0;
    std::ifstream file(filename, std::ios::binary);
    while (!file.eof())
    {
        file.read(reinterpret_cast<char*>(&m_list[i]), sizeof(m_list[i]));
        i++;
    }
    file.close();
}

/*!
 * \brief Метод измельчает сетку вдоль оси Z, количество элементов в таком случае удваивается
 */

void BoxNet::segmentation()
{
    unsigned char* list = new unsigned char[m_length * 2];
    int step = m_xSize * m_ySize;
    for (int i = 0; i < m_zSize; i++) {
        int leftEdge = 2 * i * step;
        int rightEdge = (2 * i + 1) * step;
        std::copy( m_list + step * i, m_list + step * (i + 1), list + leftEdge );
        std::copy( m_list + step * i, m_list + step * (i + 1), list + rightEdge );
    }
    delete[] m_list;
    m_list = list;
    m_zSize = m_zSize * 2;
    m_length = m_length * 2;
}
