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

//    m_leftLeg1 = new BodyPart();
//    m_leftLeg1->fillData("data/bodyparts/leftLeg-1.bin");
//    m_leftLeg1->setRotPoint( { 351, 162, 521 } );

    BodyPart * leftLeg1 = new BodyPart("data/bodyparts/leftLeg-1.bin");
    Point3D <float> rotP = { 351, 162, 521 }; // левое колено
    RotationMatrix matrix = { rotP, 0, M_PI / 2, M_PI / 2 };
    leftLeg1->setMatrix(matrix);

    BodyPart * leftHand1 = new BodyPart("data/bodyparts/leftHand-1.bin");
    RotationMatrix matrix2 = { { 443, 134, 1471 }, 0, M_PI / 2, -M_PI / 2 };
    leftHand1->setMatrix(matrix2);

    RotationMatrix matrix3 = { { 406, 134, 884 }, 0, M_PI / 2, -M_PI / 1.5 };

    leftLeg1->setMatrix(matrix3);

    m_bodyparts.push_back(leftLeg1);
    m_bodyparts.push_back(leftHand1);

//    checkBin("rightHand.bin");

    makeNet();
    rotate();

//    for (auto bp = m_bodyparts.begin(); bp != m_bodyparts.end(); bp++) {
//        check(**bp);
//    }
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

unsigned char Phantom::getValue( Point3D <float> point )
{
    int x = point.x() / m_xScale;
    int y = point.y() / m_yScale;
    int z = point.z() / m_zScale;
    m_boxNet.getByXyz(x, y, z);
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

void Phantom::setValue(Point3D <float> point, unsigned char value)
{
    int x = point.x() / m_xScale;
    int y = point.y() / m_yScale;
    int z = point.z() / m_zScale;
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

void Phantom::grow( Point3D <int> const & sizes, Point3D <int> const & position )
{
    m_boxNet.grow( sizes, position );
}

Point3D <float> Phantom::center(int num) const
{
    Point3D <int> pos = m_boxNet.getXYZ(num);
    float x = pos.x() + 0.5;
    float y = pos.y() + 0.5;
    float z = pos.z() + 0.5;
    return { x * m_xScale, y * m_yScale , z * m_zScale };
}

Point3D <float> Phantom::center(Point3D <int> const & pos) const
{
    float x = pos.x() + 0.5;
    float y = pos.y() + 0.5;
    float z = pos.z() + 0.5;
    return { x * m_xScale, y * m_yScale , z * m_zScale };
}

Point3D <float> Phantom::center(Point3D <float> const & pos) const
{
    float x = pos.x() + 0.5;
    float y = pos.y() + 0.5;
    float z = pos.z() + 0.5;
    return { x * m_xScale, y * m_yScale , z * m_zScale };
}

Point3D <float> Phantom::quarterOne(int num) const
{
    Point3D <int> pos = m_boxNet.getXYZ(num);
    float x = pos.x() + 0.5;
    float y = pos.y() + 0.5;
    float z = pos.z() + 0.25;
    return { x * m_xScale, y * m_yScale , z * m_zScale };
}

Point3D <float> Phantom::quarterTwo(int num) const
{
    Point3D <int> pos = m_boxNet.getXYZ(num);
    float x = pos.x() + 0.5;
    float y = pos.y() + 0.5;
    float z = pos.z() + 0.75;
    return { x * m_xScale, y * m_yScale , z * m_zScale };
}

void Phantom::pickRightLeg()
{
    for (int k = 0; k < 338; k++) {
        Slice slice = m_boxNet.getSliceZ(k);
        for (int j = 0; j < slice.getSizeY(); j++) {
            int skin = 0;
            for (int i = 0; i < slice.getSizeX()/2 + 3; i++) {
                unsigned char val = slice.getValue(i, j);
                if (val != 0) {
                    if (val == 125 || val == 141) {
                        skin++;
                    }
                    unsigned char nextVal = slice.getValue(i + 1, j);
                    unsigned char next2Val = slice.getValue(i + 2, j);
                    if (val == 125 && nextVal == 0 && i > 140) {
                        int num = m_boxNet.getNum(i, j, k);
//                        m_rightLeg.push_back(num);
                        break;
                    }
                    if (val == 125 && nextVal == 125 && next2Val == 119 && i > 140) {
                        int num = m_boxNet.getNum(i, j, k);
//                        m_rightLeg.push_back(num);
                        break;
                    }
                    if (i < slice.getSizeX()/2 && skin > 0) {
                        int num = m_boxNet.getNum(i, j, k);
//                        m_rightLeg.push_back(num);
                    }
                }
            }
        }
    }
//    writeBinFile("rightLeg.bin", m_rightLeg);
}

void Phantom::pickLeftLeg()
{
    for (int k = 0; k < 338; k++) {
        Slice slice = m_boxNet.getSliceZ(k);
        for (int j = 0; j < slice.getSizeY(); j++) {
            int skin = 0;
            for (int i = slice.getSizeX(); i > slice.getSizeX()/2 - 7; i--) {
                unsigned char val = slice.getValue(i, j);
                if (val != 0) {
                    if (val == 125 || val == 141) {
                        skin++;
                    }
                    unsigned char nextVal = slice.getValue(i - 1, j);
                    unsigned char next2Val = slice.getValue(i - 2, j);
                    if (val == 125 && nextVal == 0 && i < 170) {
                        int num = m_boxNet.getNum(i, j, k);
                        m_boxNet.setByNum(num, 70);
//                        m_leftLeg.push_back(num);
                        break;
                    }
                    if (val == 125 && nextVal == 125 && next2Val == 119 && i < 160) {
                        int num = m_boxNet.getNum(i, j, k);
                        m_boxNet.setByNum(num, 70);
//                        m_leftLeg.push_back(num);
                        break;
                    }
                    if (skin > 0) {
                        int num = m_boxNet.getNum(i, j, k);
                        m_boxNet.setByNum(num, 70);
//                        m_leftLeg.push_back(num);
                    }
                }
            }
        }
    }
//    writeBinFile("leftLeg.bin", m_leftLeg);
//    for(int i = 0; i < 10; i++) {
//        Logger::Instance() << m_leftLeg[m_leftLeg.size() - 1 - i] << " ";
//    }
//    Logger::Instance() << "\n";
//    Logger::Instance() << "legSize = " << m_leftLeg.size() << "\n";
}

void Phantom::pickRightHand()
{
    for (int k = 322; k < 556; k++) {
        Slice slice = m_boxNet.getSliceZ(k);
        for (int j = 0; j < slice.getSizeY(); j++) {
            for (int i = 0; i < slice.getSizeX()/2; i++) {
                unsigned char val = slice.getValue(i, j);
                if (val != 0) {
                    if (val == 123 || val == 125) break;
                    int num = m_boxNet.getNum(i, j, k);
                    m_boxNet.setByNum(num, 70);
//                    m_rightHand.push_back(num);
                }
            }
        }
    }
//    writeBinFile("rightHand.bin", m_rightHand);
}

void Phantom::pickLeftHand()
{
    std::vector <int> leftHand;
    for (int k = 316; k < 562; k++) {
        Slice slice = m_boxNet.getSliceZ(k);
        for (int j = 0; j < slice.getSizeY(); j++) {
            for (int i = slice.getSizeX() - 1; i > slice.getSizeX()/2; i--) {
                unsigned char val = slice.getValue(i, j);
                if (val != 0) {
                    if (val == 123 || val == 125) break;
                    int num = m_boxNet.getNum(i, j, k);
                    m_boxNet.setByNum(num, 70);
//                    leftHand.push_back(num);
                }
                if (k == 550 || k == 551) {
                    unsigned char nextVal = slice.getValue(i - 1, j);
                    if (val == 124 && nextVal == 117) {
                        break;
                    }
                }
            }
        }
    }
//    writeBinFile("leftHand.bin", leftHand);
}

void Phantom::writeBinFile(char const * filename, std::vector<int> const & v)
{
    std::ofstream file(filename, std::ios::binary);
    for (auto it = v.begin(); it != v.end(); it++) {
        int num = *it;
        file.write(reinterpret_cast<char const*>(&num), sizeof(num));
    }
    file.close();
}

void Phantom::readBin(char const * filename)
{
    int num = 0;
    std::ifstream file(filename, std::ios::binary);
    while (!file.eof())
    {
        file.read(reinterpret_cast<char*>(&num), sizeof(num));
//        m_leftLeg1.push_back(num);
    }
    file.close();
//    Logger::Instance() << "legSize = " << m_leftLeg1.size() << "\n";
//    for(int i = 0; i < 10; i++) {
//        Logger::Instance() << m_leftLeg1[m_leftLeg1.size() - i - 1] << " ";
//    }
//    Logger::Instance() << "\n";
}

void Phantom::checkBin(char const * filename)
{
    int num = 0;
    std::ifstream file(filename, std::ios::binary);
    while (!file.eof())
    {
        file.read(reinterpret_cast<char*>(&num), sizeof(num));
        m_boxNet.setByNum(num, 70);
    }
    file.close();
}

void Phantom::check(BodyPart const & bp)
{
    for (auto it = bp.data.begin(); it != bp.data.end(); it++) {
        m_boxNet.setByNum(*it, 70);
    }
}

void Phantom::cutBin(char const * filename, int firstEdge, int secondEdge)
{
    int num = 0, z = 0;
    int square = m_boxNet.getSizeX() * m_boxNet.getSizeY();
    char name1[40];
    char name2[40];
    char name3[40];
    sprintf(name1, "1-%s", filename);
    sprintf(name2, "2-%s", filename);
    sprintf(name3, "3-%s", filename);
    std::ifstream input(filename, std::ios::binary);
    std::ofstream output1(name1, std::ios::binary);
    std::ofstream output2(name2, std::ios::binary);
    std::ofstream output3(name3, std::ios::binary);
    while (!input.eof())
    {
        input.read(reinterpret_cast<char*>(&num), sizeof(num));
        z = num / square;
        if (z <= firstEdge) {
            output1.write(reinterpret_cast<char const*>(&num), sizeof(num));
        } else if (z <= secondEdge) {
            output2.write(reinterpret_cast<char const*>(&num), sizeof(num));
        } else {
            output3.write(reinterpret_cast<char const*>(&num), sizeof(num));
        }
    }
    input.close();
    output1.close();
    output2.close();
    output3.close();
}

void Phantom::makeNet()
{
    int minX = 1000, maxX = 0, minY = 1000, maxY = 0, minZ = 1000, maxZ = 0;

    for (auto bp = m_bodyparts.begin(); bp != m_bodyparts.end(); bp++) {
        for (auto it = (*bp)->data.begin(); it != (*bp)->data.end(); it++) {
            Point3D <float> _centerPoint = center(*it);
            for (auto pm = (*bp)->matrices.begin(); pm != (*bp)->matrices.end(); pm++) {
                RotationMatrix matrix = *pm;
                matrix.Rotate(_centerPoint);
            }
            // находим максимальные и минамальные точки
            if (minX > _centerPoint.x()) { minX = _centerPoint.x(); }
            if (maxX < _centerPoint.x()) { maxX = _centerPoint.x(); }
            if (minY > _centerPoint.y()) { minY = _centerPoint.y(); }
            if (maxY < _centerPoint.y()) { maxY = _centerPoint.y(); }
            if (minZ > _centerPoint.z()) { minZ = _centerPoint.z(); }
            if (maxZ < _centerPoint.z()) { maxZ = _centerPoint.z(); }
        }
    }

    int aX, bX, aY, bY, aZ, bZ; // превышение старых границ

    aX = (minX < 0) ? - minX / m_xScale + 1 : 0;
    aY = (minY < 0) ? - minY / m_yScale + 1 : 0;
    aZ = (minZ < 0) ? - minZ / m_zScale + 1 : 0;
    float _bX = maxX - m_xScale * m_boxNet.getSizeX();
    float _bY = maxY - m_yScale * m_boxNet.getSizeY();
    float _bZ = maxZ - m_zScale * m_boxNet.getSizeZ();
    bX = (_bX > 0) ? _bX / m_xScale + 1 : 0;
    bY = (_bY > 0) ? _bY / m_yScale + 1 : 0;
    bZ = (_bZ > 0) ? _bZ / m_zScale + 1 : 0;

    int sizeX = aX + m_boxNet.getSizeX() + bX;
    int sizeY = aY + m_boxNet.getSizeY() + bY;
    int sizeZ = aZ + m_boxNet.getSizeZ() + bZ;

    Point3D <int> size = { sizeX, sizeY, sizeZ };
    Point3D <int> position = { aX, aY, aZ };
    Point3D <float> _position = { aX * m_xScale, aY * m_yScale, aZ * m_zScale };

    m_boxNet.setNymphPos(position);
    m_boxNet.setNymphSize(size);


    for (auto bp = m_bodyparts.begin(); bp != m_bodyparts.end(); bp++) {
        for (auto pm = (*bp)->matrices.begin(); pm != (*bp)->matrices.end(); pm++) {
            Point3D <float> rotPoint = (*pm).getRotPoint();
            (*pm).setRotPoint(rotPoint + _position);
        }
        for (std::vector<int>::size_type i = 0; i != (*bp)->data.size(); i++) {
            (*bp)->data[i] = m_boxNet.translitNum((*bp)->data[i]);
        }
    }

    m_boxNet.grow(size, position);
}

void Phantom::rotate()
{
    unsigned char color;
    for (auto bp = m_bodyparts.begin(); bp != m_bodyparts.end(); bp++) {
        for (auto it = (*bp)->data.begin(); it != (*bp)->data.end(); it++) {
            color = m_boxNet.getByNum(*it);
            // Точки внутри вокселя, которые при любом повороте покроют требуемую область
            Point3D <float> firstQuarter = quarterOne(*it);
            Point3D <float> secondQuarter = quarterTwo(*it);
            // Находим кандидатов на перекрашивание
            for (auto pm = (*bp)->matrices.begin(); pm != (*bp)->matrices.end(); pm++) {
                RotationMatrix matrix = *pm;
                matrix.Rotate(firstQuarter);
                matrix.Rotate(secondQuarter);
            }

            // Находим центр этой/этих ячеек
            Point3D <float> fq = center(firstQuarter);
            Point3D <float> sq = center(secondQuarter);

            // Находим цвет !!!!TODO обратный поворот!!!!
            for (auto pm = (*bp)->matrices.begin(); pm != (*bp)->matrices.end(); pm++) {
                RotationMatrix matrix = *pm;
                matrix.Rotate(firstQuarter);
                matrix.Rotate(secondQuarter);
            }
            matrix.negaRotate(fq);
            matrix.negaRotate(sq);
            unsigned char fqCol = getValue(fq);
            unsigned char sqCol = getValue(sq);

            setValue(firstQuarter, color);
            setValue(secondQuarter, color);
//            setValue(*it, 0);
        }
    }
}

Point3D <int> Phantom::getXYZ(Point3D <float> const & point) const
{
    int x = point.x() / m_xScale;
    int y = point.y() / m_yScale;
    int z = point.z() / m_zScale;
    return { x, y, z };
}

std::ostream & operator << (std::ostream & os, Phantom const & obj)
{
    os << "|Phantom|\n\
        address = " << &obj << "\n\
        boxAddress = " << &obj.boxNet() << "\n";
    return os;
}
