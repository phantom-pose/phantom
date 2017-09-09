#include "boxnet.h"
#include "logger.h"

/*!
 * \brief Конструктор без параметров, создает пустой объект без данных
 */

BoxNet::BoxNet()
{
//    Logger::Instance() << "BoxNet default constructor" << "\n";
    m_list = nullptr;
}

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
//    m_list = new unsigned char[m_length];
//    //fill array with zeroes
//    for (int i = 0; i < m_length; i++) {
//        m_list[i] = 0;
//    }

    m_list = new unsigned char**[sizeX];
    for (int ix = 0; ix < sizeX; ix++) {
        m_list[ix] = new unsigned char*[sizeY];
        for (int iy = 0; iy < sizeY; iy++) {
            m_list[ix][iy] = new unsigned char[sizeZ];
            for (int iz = 0; iz < sizeZ; iz++) {
                m_list[ix][iy][iz] = 0;
            }
        }
    }

//    Logger::Instance() << "BoxNet simple constructor" << "\n";
}

/*!
 * \brief Конструктор копирования
 * \param obj копируемый объект
 */

BoxNet::BoxNet( BoxNet const & obj )
    : m_xSize(obj.m_xSize), m_ySize(obj.m_ySize), m_zSize(obj.m_zSize)
{
//    m_length = obj.m_length;
//    m_list = new unsigned char[m_length];
//    std::copy( obj.m_list, obj.m_list + obj.m_length, m_list );

    m_length = obj.m_length;
    m_list = new unsigned char**[m_xSize];
    for (int ix = 0; ix < m_xSize; ix++) {
        m_list[ix] = new unsigned char*[m_ySize];
        for (int iy = 0; iy < m_ySize; iy++) {
            m_list[ix][iy] = new unsigned char[m_zSize];
            std::copy( obj.m_list[ix][iy], obj.m_list[ix][iy] + obj.m_zSize, m_list[ix][iy]);
        }
    }
//    Logger::Instance() << "BoxNet copy constructor" << "\n";
}

/*!
 * \brief Деструктор, освобождает память массива m_list
 */

BoxNet::~BoxNet()
{
//    delete[] m_list;
    for (int ix = 0; ix < m_xSize; ix++) {
        for (int iy = 0; iy < m_ySize; iy++) {
            delete[] m_list[ix][iy];
        }
        delete[] m_list[ix];
    }
    delete[] m_list;
}

/*!
 * \brief Метод возвращает значение ячейки по порядковому номеру
 * \param num Порядковый номер ячейки
 * \return unsigned char элемент - id органа
 */

unsigned char const & BoxNet::getByNum(int num) const
{
//    return m_list[num];
    int z = num / ( m_xSize * m_ySize );
    int y = (num - z * ( m_xSize * m_ySize )) / m_xSize;
    int x = (num - z * ( m_xSize * m_ySize )) % m_xSize;
    return m_list[x][y][z];
}

/*!
 * \brief Метод возвращает значение ячейки по её координатам x y z в сетке
 * \param x Координата X
 * \param y Координата Y
 * \param z Координата Z
 * \return unsigned char элемент - id органа
 */

unsigned char const & BoxNet::getByXyz(int x, int y, int z) const
{
//    int num = z * m_xSize * m_ySize + y * m_xSize + x;
//    return m_list[num];
    return m_list[x][y][z];
}

/*!
 * \brief Метод позволяет установить значение ячейки по порядковому номеру num
 * \param num Порядковый номер ячейки
 * \param value Значение unsigned char для помещения в ячейку
 */

void BoxNet::setByNum(int num, unsigned char value)
{
//    m_list[num] = value;
    int z = num / ( m_xSize * m_ySize );
    int y = (num - z * ( m_xSize * m_ySize )) / m_xSize;
    int x = (num - z * ( m_xSize * m_ySize )) % m_xSize;
    m_list[x][y][z] = value;
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
//    int num = z * m_xSize * m_ySize + y * m_xSize + x;
//    m_list[num] = value;
    m_list[x][y][z] = value;
}

/*!
 * \brief Метод для получения размера сетки по оси X
 * \return Размер сетки по оси X
 */

int const & BoxNet::getSizeX() const { return m_xSize; }

/*!
 * \brief Метод для получения размера сетки по оси Y
 * \return Размер сетки по оси Y
 */

int const & BoxNet::getSizeY() const { return m_ySize; }

/*!
 * \brief Метод для получения размера сетки по оси Z
 * \return Размер сетки по оси Z
 */

int const & BoxNet::getSizeZ() const { return m_zSize; }

/*!
 * \brief Метод возвращает значение Slice для среза по оси X
 * \param num Положение среза вдоль оси Х
 * \return Slice элемент
 */

Slice BoxNet::getSliceX(int num) const
{
    if (num < 0 || num > m_xSize) {
        Slice slice;
        return slice;
    }
    Slice slice = { m_ySize, m_zSize };

    for (int iz = 0; iz < m_zSize; iz++)
    {
        for (int iy = 0; iy < m_ySize; iy++)
        {
//            int n = iz * m_xSize * m_ySize + iy * m_xSize + num;
//            slice.setValue(iy, iz, m_list[n]);
            slice.setValue(iy, iz, m_list[num][iy][iz]);
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
//            int n = iz * m_xSize * m_ySize + num * m_xSize + ix;
//            slice.setValue(ix, iz, m_list[n]);
            slice.setValue(ix, iz, m_list[ix][num][iz]);
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
//            int n = num * m_xSize * m_ySize + iy * m_xSize + ix;
//            slice.setValue(ix, iy, m_list[n]);
            slice.setValue(ix, iy, m_list[ix][iy][num]);
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
//                m_list[i++] = (unsigned char)ival;
                int z = i / ( m_xSize * m_ySize );
                int y = (i - z * ( m_xSize * m_ySize )) / m_xSize;
                int x = (i - z * ( m_xSize * m_ySize )) % m_xSize;
                m_list[x][y][z] = (unsigned char)ival;
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
//    int i = 0;
    std::ofstream file(filename, std::ios::binary);
//    while (i < m_xSize * m_ySize * m_zSize)
//    {
//        file.write(reinterpret_cast<char const*>(&m_list[i]), sizeof(m_list[i]));
//        i++;
//    }
    for (int iz = 0; iz < m_zSize; iz++) {
        for (int iy = 0; iy < m_ySize; iy++) {
            for (int ix = 0; ix < m_xSize; ix++) {
                file.write(reinterpret_cast<char const*>(&m_list[ix][iy][iz]), sizeof(m_list[ix][iy][iz]));
            }
        }
    }
    file.close();
}

/*!
 * \brief Метод заполнения сетки из двоичного файла
 * \param filename имя файла, относительный путь
 */

void BoxNet::fillFromBin(char const * filename)
{
//    int i = 0;
    std::ifstream file(filename, std::ios::binary);
//    while (!file.eof())
//    {
//        file.read(reinterpret_cast<char*>(&m_list[i]), sizeof(m_list[i]));
//        i++;
//    }
    for (int iz = 0; iz < m_zSize; iz++) {
        for (int iy = 0; iy < m_ySize; iy++) {
            for (int ix = 0; ix < m_xSize; ix++) {
                file.read(reinterpret_cast<char*>(&m_list[ix][iy][iz]), sizeof(m_list[ix][iy][iz]));
            }
        }
    }
    file.close();
}

/*!
 * \brief Метод измельчает сетку вдоль оси Z, количество элементов в таком случае удваивается
 */

void BoxNet::segmentation()
{
//    unsigned char* list = new unsigned char[m_length * 2];
//    int step = m_xSize * m_ySize;
//    for (int i = 0; i < m_zSize; i++) {
//        int leftEdge = 2 * i * step;
//        int rightEdge = (2 * i + 1) * step;
//        std::copy( m_list + step * i, m_list + step * (i + 1), list + leftEdge );
//        std::copy( m_list + step * i, m_list + step * (i + 1), list + rightEdge );
//    }
//    delete[] m_list;
    unsigned char *** list = new unsigned char**[m_xSize];
    for (int ix = 0; ix < m_xSize; ix++) {
        list[ix] = new unsigned char*[m_ySize];
        for (int iy = 0; iy < m_ySize; iy++) {
            list[ix][iy] = new unsigned char[m_zSize * 2];
            for (int iz = 0; iz < m_zSize; iz++) {
                int i1 = iz * 2;
                int i2 = iz * 2 + 1;
                list[ix][iy][i1] = m_list[ix][iy][iz];
                list[ix][iy][i2] = m_list[ix][iy][iz];
            }
        }
    }
    for (int ix = 0; ix < m_xSize; ix++) {
        for (int iy = 0; iy < m_ySize; iy++) {
            delete[] m_list[ix][iy];
        }
        delete[] m_list[ix];
    }
    delete[] m_list;

    m_list = list;
    m_zSize = m_zSize * 2;
    m_length = m_length * 2;
}

/*!
 * \brief Оператор присваивания
 * \param obj Присваеваемый объект
 */

BoxNet &  BoxNet::operator = ( BoxNet const & obj )
{
    if ( this == &obj ) return *this;
    m_xSize = obj.m_xSize;
    m_ySize = obj.m_ySize;
    m_zSize = obj.m_zSize;
    m_length = obj.m_length;
//    m_list = new unsigned char[m_length];
//    std::copy( obj.m_list, obj.m_list + obj.m_length, m_list );
    m_list = new unsigned char**[m_xSize];
    for (int ix = 0; ix < m_xSize; ix++) {
        m_list[ix] = new unsigned char*[m_ySize];
        for (int iy = 0; iy < m_ySize; iy++) {
            m_list[ix][iy] = new unsigned char[m_zSize];
            std::copy( obj.m_list[ix][iy], obj.m_list[ix][iy] + obj.m_zSize, m_list[ix][iy]);
        }
    }
//    Logger::Instance() << "BoxNet = operator" << "\n";
    return *this;
}

int const & BoxNet::getLength() const
{
    return m_length;
}

bool BoxNet::operator == (BoxNet const & obj) const
{
    bool equal = 1;
    if ( m_xSize != obj.m_xSize || m_ySize != obj.m_ySize || m_zSize != obj.m_zSize
        || m_length != obj.m_length ) return 0;
//    for (int i = 0; i < m_length; i++) {
//        equal = ( m_list[i] == obj.m_list[i] );
//    }
    for (int ix = 0; ix < m_xSize; ix++) {
        for (int iy = 0; iy < m_ySize; iy++) {
            for (int iz = 0; iz < m_zSize; iz++) {
                equal = ( m_list[ix][iy][iz] == obj.m_list[ix][iy][iz] );
            }
        }
    }
    return equal;
}

Point3D <int> const & BoxNet::position() const
{
    return m_position;
}

void  BoxNet::setPosition( Point3D <int> const & obj )
{
    m_position = obj;
}

void  BoxNet::setNymphSize( Point3D <int> const & obj )
{
    m_nymphSize = obj;
}

void  BoxNet::setNymphPos( Point3D <int> const & obj )
{
    m_nymphPos = obj;
}

void BoxNet::grow( Point3D <int> const & sizes, Point3D <int> const & position )
{
    int dataSizeX = sizes.x();
    int dataSizeY = sizes.y();
    int dataSizeZ = sizes.z();
    int dataLength = dataSizeX * dataSizeY * dataSizeZ;

    // Выделяем память под новый массив
//    unsigned char * data = new unsigned char [ dataLength ];
    unsigned char *** data = new unsigned char ** [ dataSizeX ];


    // Заполняем его нулями
//    for (int i = 0; i < dataLength; i++) {
//        data[i] = 0;
//    }
    for (int ix = 0; ix < dataSizeX; ix++) {
        data[ix] = new unsigned char*[dataSizeY];
        for (int iy = 0; iy < dataSizeY; iy++) {
            data[ix][iy] = new unsigned char[dataSizeZ];
            for (int iz = 0; iz < dataSizeZ; iz++) {
                data[ix][iy][iz] = 0;
            }
        }
    }

    int posX = position.x();
    int posY = position.y();
    int posZ = position.z();

    int curPosX = 0;
    int curPosY = 0;
    int curPosZ = 0;

//    int n = 0;
    unsigned char value = 0;
//    int nData = 0;

    for (int iz = 0; iz < m_zSize; iz++) {
        for (int iy = 0; iy < m_ySize; iy++) {
            for (int ix = 0; ix < m_xSize; ix++) {
                curPosX = ix + posX;
                curPosY = iy + posY;
                curPosZ = iz + posZ;
//                value = m_list[n];
                value = m_list[ix][iy][iz];
//                nData = curPosZ * dataSizeX * dataSizeY + curPosY * dataSizeX + curPosX;
//                data[nData] = value;
                data[curPosX][curPosY][curPosZ] = value;
//                n++;
            }
        }
    }

//    delete[] m_list;
    for (int ix = 0; ix < m_xSize; ix++) {
        for (int iy = 0; iy < m_ySize; iy++) {
            delete[] m_list[ix][iy];
        }
        delete[] m_list[ix];
    }
    delete[] m_list;

    m_list = data;
    m_xSize = dataSizeX;
    m_ySize = dataSizeY;
    m_zSize = dataSizeZ;
    m_length = dataLength;
}

Point3D <int> BoxNet::getXYZ(int num) const
{
    int z = num / ( m_xSize * m_ySize );
    int y = (num - z * ( m_xSize * m_ySize )) / m_xSize;
    int x = (num - z * ( m_xSize * m_ySize )) % m_xSize;
    return { x, y, z };
}

int BoxNet::getNum(int x, int y, int z)
{
    return z * m_xSize * m_ySize + y * m_xSize + x;
}

int BoxNet::translitNum(int num)
{
    int z = num / ( m_xSize * m_ySize );
    int y = (num - z * ( m_xSize * m_ySize )) / m_xSize;
    int x = (num - z * ( m_xSize * m_ySize )) % m_xSize;

    int _x = x + m_nymphPos.x();
    int _y = y + m_nymphPos.y();
    int _z = z + m_nymphPos.z();

    return _z * m_nymphSize.x() * m_nymphSize.y() + _y * m_nymphSize.x() + _x;
}

int BoxNet::getSize(int const & index) const
{
    if (index == 0)
        return m_xSize;
    if (index == 1)
        return m_ySize;
    if (index == 2)
        return m_zSize;
    return 0;
}

std::ostream & operator << (std::ostream & os, BoxNet const & obj)
{
    os << "|BoxNet|\n\
        address = " << &obj << "\n\
        sizes   = { " << obj.getSizeX() << " " << obj.getSizeY() << " " << obj.getSizeZ() << " }\n\
        length  = " << obj.getLength() << "\n";
//    if (obj.getLength() >= 3) {
//        os << "\
//            first 3 = { " << (int)obj.getByNum(0) << " " << (int)obj.getByNum(1) << " " << (int)obj.getByNum(2) << " }\n\
//            last  3 = { " << (int)obj.getByNum(obj.getLength()-3) << " " << (int)obj.getByNum(obj.getLength()-2) << " "
//                            << (int)obj.getByNum(obj.getLength()-1) << " }\n";
//    }
    return os;
}
