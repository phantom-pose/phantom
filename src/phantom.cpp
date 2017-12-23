#include "phantom.h"
#include "logger.h"

/*!
 * \brief Конструктор без параметров
 * \details Создаёт объект класса Phantom с данными из файла AF_bin, раздроблённый в 2 раза по оси Z.
 * Параметры по размерам ячеек: 1.875 x 1.875 x 2.5 мм..
 */

Phantom::Phantom()
{
//    m_boxNet = { 299, 137, 348 };
    m_boxNet = { 299, 437, 696 };
//    m_boxNet.fillFromBin( "data/AF_bin.dat" );
    m_boxNet.fillFromBin( "data/savePhantom.bin" );
    saveSurface("../sitting");
//    m_boxNet.segmentation();
//    m_boxNet = { 100, 117, 96 };
//    m_boxNet.fillFromBin( "../Head.bin" );

//    BoxNet b = m_boxNet.cut( {100, 0, 598}, {200, 117, 694} );
//    b.writeBinFile("Head.bin");
//    b.writeRGBA("../rgbaHead.raw");

    // Заполняем массив точек поворота из Json файла
    // **********************************************************************
    Json::Value root;
    std::ifstream file("data/rotPoints.json", std::ifstream::binary);
    file >> root;
    file.close();
    Json::Value const & points = root;
    for (unsigned int i = 0; i < points.size(); i++) {
        Point3D <float> p = { points[i]["point"][0].asFloat(), points[i]["point"][1].asFloat(), points[i]["point"][2].asFloat() };
        m_rotpoints[i] = p;
    }
    // **********************************************************************
    cutBodyparts();
    loadScenario();
    Point3D <int> point;

//    m_rightKnee = RightKnee(m_boxNet, M_PI/2, M_PI/2, &point);
//    BoxNet delHip = RightHip(m_boxNet, 0, &point);
//    m_rightHip = RightHip(m_boxNet, M_PI/2, &point);
//    m_boxNet.difference(delHip);
//    m_leftKnee = LeftKnee(m_boxNet, M_PI/2, M_PI/2, &point);
//    m_rightElbow = RightElbow(m_boxNet, M_PI/2, 0, 0, &point);
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
    return m_boxNet.getByNum(num);
}

unsigned char Phantom::getValue( Point3D <float> point )
{
    int x = point.x() / m_xScale;
    int y = point.y() / m_yScale;
    int z = point.z() / m_zScale;
    return m_boxNet.getByXyz(x, y, z);
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
    return m_boxNet.getByXyz(x, y, z);
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

void Phantom::setBox(BoxNet const & box)
{
    m_boxNet = box;
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
    Point3D <int> c = getXYZ(pos);
    return center(c);
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
//                        int num = m_boxNet.getNum(i, j, k);
//                        m_rightLeg.push_back(num);
                        break;
                    }
                    if (val == 125 && nextVal == 125 && next2Val == 119 && i > 140) {
//                        int num = m_boxNet.getNum(i, j, k);
//                        m_rightLeg.push_back(num);
                        break;
                    }
                    if (i < slice.getSizeX()/2 && skin > 0) {
//                        int num = m_boxNet.getNum(i, j, k);
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

void Phantom::cutBin(char const * filename, int firstEdge, int secondEdge, char const * newname)
{
    int num = 0, z = 0;
    int square = m_boxNet.getSizeX() * m_boxNet.getSizeY();
    char name1[40];
    char name2[40];
    char name3[40];
    sprintf(name1, "%s-1.bin", newname);
    sprintf(name2, "%s-2.bin", newname);
    sprintf(name3, "%s-3.bin", newname);
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

void Phantom::cutBin(char const * filename, int firstEdge, int secondEdge, int thirdEdge, char const * newname)
{
    int num = 0, z = 0;
    int square = m_boxNet.getSizeX() * m_boxNet.getSizeY();
    char name1[40];
    char name2[40];
    char name3[40];
    sprintf(name1, "%s-1.bin", newname);
    sprintf(name2, "%s-2.bin", newname);
    sprintf(name3, "%s-3.bin", newname);
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
        } else if (z <= thirdEdge) {
            output3.write(reinterpret_cast<char const*>(&num), sizeof(num));
        }
    }
    input.close();
    output1.close();
    output2.close();
    output3.close();
}

void Phantom::cutBodyparts()
{
    Json::Value rootJoint;
    std::ifstream jointfile("data/jointParams.json", std::ifstream::binary);
    jointfile >> rootJoint;
    jointfile.close();

    // Правая коленка
    int rKz1 = rootJoint["rightKnee"]["z1"].asInt();
    int rKz2 = rootJoint["rightKnee"]["z2"].asInt();
    // Учитываем также нижнюю часть правого бедра
    int rHz1 = rootJoint["rightHip"]["z1"].asInt();
    cutBin("data/bodyparts/rightLeg.bin", rKz1, rKz2-1, rHz1, "data/bodyparts/rightLeg");
    // Левая
    int lKz1 = rootJoint["leftKnee"]["z1"].asInt();
    int lKz2 = rootJoint["leftKnee"]["z2"].asInt();
    cutBin("data/bodyparts/leftLeg.bin", lKz1, lKz2-1, "data/bodyparts/leftLeg");
    // Правый локоть
    int rEz1 = rootJoint["rightElbow"]["z1"].asInt();
    int rEz2 = rootJoint["rightElbow"]["z2"].asInt();
    cutBin("data/bodyparts/rightHand.bin", rEz1, rEz2-1, "data/bodyparts/rightHand");
}

void Phantom::combineBin(char const * filename1, char const * filename2, char const * filename3, char const * out)
{
    int num = 0;

    std::ofstream output(out, std::ios::binary);
    std::ifstream input1(filename1, std::ios::binary);
    std::ifstream input2(filename2, std::ios::binary);
    std::ifstream input3(filename3, std::ios::binary);
    while (!input1.eof())
    {
        input1.read(reinterpret_cast<char*>(&num), sizeof(num));
        output.write(reinterpret_cast<char const*>(&num), sizeof(num));
    }
    while (!input2.eof())
    {
        input2.read(reinterpret_cast<char*>(&num), sizeof(num));
        output.write(reinterpret_cast<char const*>(&num), sizeof(num));
    }
    while (!input3.eof())
    {
        input3.read(reinterpret_cast<char*>(&num), sizeof(num));
        output.write(reinterpret_cast<char const*>(&num), sizeof(num));
    }

    output.close();
    input1.close();
    input2.close();
    input3.close();
}

void Phantom::makeNet()
{
    int minX = 10000, maxX = 0, minY = 10000, maxY = 0, minZ = 10000, maxZ = 0;

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
        // Пересчёт точки поворота матрицы
        for (auto pm = (*bp)->matrices.begin(); pm != (*bp)->matrices.end(); pm++) {
            Point3D <float> rotPoint = (*pm).getRotPoint();
            (*pm).setRotPoint(rotPoint + _position);
        }
        // Пересчёт номеров контента конечностей
        for (std::vector<int>::size_type i = 0; i != (*bp)->data.size(); i++) {
            (*bp)->data[i] = m_boxNet.translitNum((*bp)->data[i]);
        }
        // Пересчёт точек начала примитивов
        (*bp)->shiftPrimitivePosition(_position);
    }
    for (auto pbb = m_costume.begin(); pbb != m_costume.end(); pbb++) {
        Point3D <float> pos = (*pbb)->getPosition();
        pos = pos + _position;
        (*pbb)->setPosition(pos);
    }
    m_nymph = m_nymph + position;
    m_rightKnee.shiftPos(position);
    m_leftKnee.shiftPos(position);
    m_rightElbow.shiftPos(position);
    m_rightHip.shiftPos(position);
    Point3D <int> pos = m_rightElbow.position();
    std::cout << "position in m_rightElbow after makeNet" << pos << std::endl;
//    Point3D <int> pos = m_rightKnee.position();
//    std::cout << "position in m_leftknee after makeNet" << pos << std::endl;
//    pos = m_leftKnee.position();
//    std::cout << "position in m_leftknee after makeNet" << pos << std::endl;
    m_boxNet.grow(size, position);
}

void Phantom::rotate()
{
    unsigned char color, color1, color2;
    for (auto bp = m_bodyparts.begin(); bp != m_bodyparts.end(); bp++) {
        (*bp)->rotatePrimitive();
        if ( !(*bp)->hasJoint() ) {
            for (auto it = (*bp)->data.begin(); it != (*bp)->data.end(); it++) {
                color = m_boxNet.getByNum(*it);
                // Точки внутри вокселя, которые при любом повороте покроют требуемую область - 'четвертаки'
                Point3D <float> firstQuarter = quarterOne(*it);
                Point3D <float> secondQuarter = quarterTwo(*it);
                // Находим кандидатов на перекрашивание
                for (auto pm = (*bp)->matrices.begin(); pm != (*bp)->matrices.end(); pm++) {
                    RotationMatrix matrix = *pm;
                    matrix.Rotate(firstQuarter);
                    matrix.Rotate(secondQuarter);
                }

                // Находим центры ячеек куда попали четвертаки
                Point3D <float> fq = center(firstQuarter);
                Point3D <float> sq = center(secondQuarter);
                // Копируем их
                Point3D <float> _fq = fq;
                Point3D <float> _sq = sq;
                // И смотрим чем они были до поворота
                for (auto pm = (*bp)->matrices.rbegin(); pm != (*bp)->matrices.rend(); pm++) {
                    RotationMatrix matrix = *pm;
                    matrix.negaRotate(fq);
                    matrix.negaRotate(sq);
                }
                // Находим их прежний цвет
                color1 = getValue(fq);
                color2 = getValue(sq);

                setValue(_fq, color1);
    //            std::cout << fq;
                setValue(_sq, color2);
    //            setValue(*it, 0);
            }
        }
    }
    // Затираем (обнуляем) старые воксели
    for (auto bp = m_bodyparts.begin(); bp != m_bodyparts.end(); bp++) {
        for (auto it = (*bp)->data.begin(); it != (*bp)->data.end(); it++) {
            setValue(*it, 0);
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

void Phantom::loadScenario()
{
    Json::Value root;
    std::ifstream file("data/scenario.json", std::ifstream::binary);
    file >> root;
    file.close();
    Json::Value const & bodyparts = root;
    // Итерируемся по частям тела
    for (unsigned int i = 0; i < bodyparts.size(); i++) {
        // Извлекаем имя файла для того, чтобы создать объект бадипарт
        char const * filename = bodyparts[i]["filename"].asCString();
        // Извлекаем бул, который показывает будет ли поворачиваться data
        bool hasJoint = static_cast<bool>(bodyparts[i]["hasJoint"].asInt());
        // Создаём указатель на бадипарт, впоследствии будет находиться в m_bodyparts. Загружаем данные из файла
        BodyPart * bodypart = new BodyPart(filename);
        bodypart->sethasJoint(hasJoint);
        Json::Value const & matrices = bodyparts[i]["matrices"];
        // Итерируемся по матрицам внутри стека матриц
        for (unsigned int j = 0; j < matrices.size(); j++) {
            // Номер индекса необходимой точки поворота из m_rotpoints
            int pNum = matrices[j]["point"].asInt();
            // Углы для ориентации вектора поворота, вокруг которого будет происходить вращение (в градусах)
            float phi = matrices[j]["phi"].asFloat();
            float theta = matrices[j]["theta"].asFloat();
            // Переводим градусы в радианы
            phi *= M_PI / 180;
            theta *= M_PI / 180;
            // Угол поворота конечности
            float alpha = matrices[j]["alpha"].asFloat();
            // Переведём в радианы
            alpha *= M_PI / 180;
            // Создаём нужную матрицу
            RotationMatrix matrix = { m_rotpoints[pNum], phi, theta, alpha };
            // Добавляем матрицу в стек текущего бадипарт объекта
            bodypart->setMatrix(matrix);
        }
        Json::Value const & primitives = bodyparts[i]["primitives"];
        for (unsigned int k = 0; k < primitives.size(); k++) {
            float x0 = primitives[k]["x0"].asFloat();
            float y0 = primitives[k]["y0"].asFloat();
            float z0 = primitives[k]["z0"].asFloat();
            float a = primitives[k]["a"].asFloat();
            float b = primitives[k]["b"].asFloat();
            float c = primitives[k]["c"].asFloat();
            std::string name = primitives[k]["name"].asString();
            bodypart->setPrimitive(x0, y0, z0, a, b, c, name);
        }
        // Добавляем бадипарт в вектор частей тела
        m_bodyparts.push_back(bodypart);
    }

    Json::Value costume;
    std::ifstream cosfile("data/costume.json", std::ifstream::binary);
    cosfile >> costume;
    cosfile.close();
    Json::Value const & parts = costume;
    // Итерируемся по неподвижным частям
    for (unsigned int i = 0; i < parts.size(); i++) {
        float x0 = parts[i]["x0"].asFloat();
        float y0 = parts[i]["y0"].asFloat();
        float z0 = parts[i]["z0"].asFloat();
        float a = parts[i]["a"].asFloat();
        float b = parts[i]["b"].asFloat();
        float c = parts[i]["c"].asFloat();
        std::string name = parts[i]["name"].asString();
        m_costume.push_back(new BoundingBox(x0, y0, z0, a, b, c, name));
    }
//    fillCostume();
//    serializeCostume("../primaryCostume.json");
}

void Phantom::executeScenario()
{
    makeNet();
    rotate();
    fillCostume();
    serializeCostume("../sittingCostume.json");
//    std::cout << "BOXNET PARAMETERS" << std::endl;
//    std::cout << "size = { " << m_rightKnee.getSizeX() << " " << m_rightKnee.getSizeY() << " " << m_rightKnee.getSizeZ() << " }\n";
//    std::cout << "position = " << m_nymph << "\n";
//    std::cout << "sizeTHIS = { " << m_boxNet.getSizeX() << " " << m_boxNet.getSizeY() << " " << m_boxNet.getSizeZ() << " }\n";

//    m_boxNet.insert(m_rightKnee);
//    m_boxNet.insert(m_leftKnee);
//    m_boxNet.insert(m_rightElbow);
//    m_boxNet.insert(m_rightHip);
//    saveBin();
}

void Phantom::fillCostume()
{
    for (auto bp = m_bodyparts.begin(); bp != m_bodyparts.end(); bp++) {
        (*bp)->appendToVector( m_costume );
    }
}

void Phantom::printLegPlanes()
{
    std::cout << "plane 1" << "\n";
    std::cout << "pos1" << "\n";
    std::cout << m_costume[3]->getPos(210) << "\n";
    std::cout << "Ex1" << "\n";
    std::cout << m_costume[3]->getEx(210) << "\n";
    std::cout << "Ey1" << "\n";
    std::cout << m_costume[3]->getEy(210) << "\n";
    std::cout << "plane 2" << "\n";
    std::cout << "pos2" << "\n";
    std::cout << m_costume[2]->getPos(442) << "\n";
    std::cout << "Ex2" << "\n";
    std::cout << m_costume[2]->getEx(442) << "\n";
    std::cout << "Ey2" << "\n";
    std::cout << m_costume[2]->getEy(442) << "\n";
}

void Phantom::dumpCostume() {
    for (auto bp = m_costume.begin(); bp != m_costume.end(); bp++) {
        BoundingBox bb = **bp;
        std::cout << bb << std::endl;
    }
}

void Phantom::serializeCostume( char const * filename )
{
//    Json::Value root;
//    std::ofstream file("costume.json");
    std::vector <IJsonSerializable *>  costume(m_costume.begin(), m_costume.end());
    std::string output;
    bool err = CJsonSerializer::Serialize(costume, output);
    std::ofstream out(filename);
    out << output;
    out.close();
//    Json::StyledStreamWriter writer;
//    writer.write(file, root);
//    file.close();
}

void Phantom::deserializeCostume()
{
    std::vector <BoundingBox *>  costume;
    bool err = CJsonSerializer::Deserialize(costume, "data/serCostume.json");

    std::string output;
    std::vector <IJsonSerializable *>  costume2(costume.begin(), costume.end());
    err = CJsonSerializer::Serialize(costume2, output);
    std::cout << output;
}

void Phantom::rightKneeRotate()
{
    Point3D <int> point;
    m_rightKnee = RightKnee(m_boxNet, M_PI/2, M_PI/2, &point);
    m_nymph = m_nymph + point;
}

void Phantom::saveBin()
{
    m_boxNet.writeBinFile("data/savePhantom.bin");
    std::cout << m_boxNet;
//    ofstream outputFile;
//    char buf[256];
//    outputFile.open("data/phantomParams.txt", std::ofstream::out | std::ofstream::trunc);
//    std::sprintf(buf, "xSize = %d\nySize = %d\nzSize = %d\n", m_boxNet.getSizeX(), m_boxNet.getSizeY(), m_boxNet.getSizeZ());
//    outputFile << buf;
//    outputFile.close();
}

void Phantom::saveSurface( std::string const & filename )
{
    std::size_t found = filename.find_last_of("/\\");
    Json::Value Root;
    Root["sizeX"] = m_boxNet.getSizeX();
    Root["sizeY"] = m_boxNet.getSizeY();
    Root["sizeZ"] = m_boxNet.getSizeZ();
    Root["propX"] = m_xScale;
    Root["propY"] = m_yScale;
    Root["propZ"] = m_zScale;
    Root["name"]  = filename.substr(found+1);


    int counter = 0;
    bool contour = true;
    std::string name = filename + ".bin";
    std::ofstream binfile(name, std::ios::binary);
    for (int iz = 0; iz < m_boxNet.getSizeZ(); iz++) {
        for (int iy = 0; iy < m_boxNet.getSizeY(); iy++) {
            for (int ix = 0; ix < m_boxNet.getSizeX(); ix++) {
                unsigned char value = m_boxNet.getByXyz(ix, iy, iz);
                if ( static_cast<bool>(value) == contour ) {
                    counter++;
                    contour = !contour;
                    float x = ix * m_xScale;
                    float y = (static_cast<float>(iy) + 0.5f) * m_yScale;
                    float z = (static_cast<float>(iz) + 0.5f) * m_zScale;
//                    m_boxNet.setByXyz(ix, iy, iz, 141);
                    binfile.write(reinterpret_cast<char const*>(&x), sizeof(x));
                    binfile.write(reinterpret_cast<char const*>(&y), sizeof(y));
                    binfile.write(reinterpret_cast<char const*>(&z), sizeof(z));
                }
            }
        }
    }
    binfile.close();

    Root["counter"]  = counter;
    Json::StyledWriter writer;
    std::string output = writer.write( Root );
    std::string fn = filename + ".json";
    std::ofstream out(fn);
    out << output;
    out.close();
}
