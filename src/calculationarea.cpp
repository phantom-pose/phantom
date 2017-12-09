#include "calculationarea.hpp"

CalculationArea::CalculationArea( BoxNet const & box )
{
    m_boxNet = { box };
    float xLen = m_xScale * m_boxNet.getSizeX();
    float yLen = m_yScale * m_boxNet.getSizeY();
    float zLen = m_zScale * m_boxNet.getSizeZ();
    m_boxSize = { xLen, yLen, zLen };

    m_planes.push_back({ { 0, 0, 0 }, { 0, 0, 1 } });
    m_planes.push_back({ { 0, 0, 0 }, { 0, 1, 0 } });
    m_planes.push_back({ { 0, 0, 0 }, { 1, 0, 0 } });
    m_planes.push_back({ { xLen, 0, 0 }, { -1, 0, 0 } });
    m_planes.push_back({ { 0, yLen, 0 }, { 0, -1, 0 } });
    m_planes.push_back({ { 0, 0, zLen }, { 0, 0, -1 } });

    CJsonSerializer::Deserialize(m_costume, "data/stdCostume.json");
}

unsigned char CalculationArea::getValue( Point3D <float> point )
{
    int x = point.x() / m_xScale;
    int y = point.y() / m_yScale;
    int z = point.z() / m_zScale;
    return m_boxNet.getByXyz(x, y, z);
}

void CalculationArea::setBox(BoxNet const & box)
{
    m_boxNet = box;
}

bool CalculationArea::hasInsideBox(Vector3D v) {
    Point3D <float> origin = v.getPosition();
    Point3D <float> zero;
    return origin > zero && origin < m_boxSize;
}

bool CalculationArea::hasInsideBox(Point3D <float> const & pt) {
    Point3D <float> zero;
    return pt > zero && pt < m_boxSize;
}

// Для луча снаружи основного блока
int CalculationArea::prepLineOut(Line & line, float & il) {
    float len = 0.0f;
    float lens[2];
    int i = 0;
    for (auto it = m_planes.begin(); it != m_planes.end(); it++) {
        int err = linePlaneIntersect(len, line, *it);
        //std::cout << "err = " << err << "\n";
        // Если хоть какие-нибудь плоскости были пересечены, то функция уже отработала нормально => code = 0
        if (!err) {
            lens[i] = len;
            i++;
        }
    }
    if (i == 2) {
        // На данном этапе в templen и len ближняя и дальняя точки, но неизвестно где какая
        if (lens[1] > lens[0]) {
            il = lens[0];
            line.shiftPosition(lens[0]);
            line.setMaxLen(lens[1]-lens[0]);
        } else {
            il = lens[1];
            line.shiftPosition(lens[1]);
            line.setMaxLen(lens[0]-lens[1]);
        }
        //std::cout << lens[0] << " " << lens[1] << " " << i << "\n";
        return 0;
    }
    return 1;
}

// Для луча внутри основного блока
int CalculationArea::prepLineIn(Line & line, float & il) {
    float len = 0.0f;
    int i = 0;
    for (auto it = m_planes.begin(); it != m_planes.end(); it++) {
        int err = linePlaneIntersect(len, line, *it);
        // Если хоть какие-нибудь плоскости были пересечены, то функция уже отработала нормально => code = 0
        if (!err) {
            i++;
        }
    }
    if (i == 1) {
        line.setMaxLen(len);
        il = 0;
        //std::cout << lens[0] << " " << lens[1] << " " << i << "\n";
        return 0;
    }
    return 1;
}

int CalculationArea::linePlaneIntersect(float & len, Line const & line, Vector3D const & plane) {

    Point3D <float> u = line.getDirection();
    Point3D <float> w = line.getPosition() - plane.getPosition();
    float D = plane.getDirection() * u;
    float N = -(plane.getDirection() * w);

    if (fabs(D) < kEps) {
        if (fabs(N) < kEps)
            return 1; // луч принадлежит плоскости
        else
            return 2; // луч параллелен плоскости
    }
    float s = N / D;
    if (s < 0)
        return 3; // луч направлен в другую от плоскости сторону
    Point3D <float> point = line.getPosition() + u * s;
    Point3D <float> checkdot = point + plane.getDirection(); // немного смещаемся в сторону нормали для оценки попадания внутрь
    //std::cout << point << "\n";
    //std::cout << checkdot << "\n";
    if ( hasInsideBox(checkdot) ) { // если точка принадлежит грани основного блока
        len = s;
        return 0;
    }
    return 4; // плоскость пересекает, но за пределами коробки
}

void CalculationArea::startIterations(Line & line) {
//    int p = 0;

    // Извлеку максимально возможную длину из line для проверок выхода за пределы блока
    float stopLen = line.getMaxLen() - 0.001;
    // Запишу направление | НЕ МЕНЯЕТСЯ |
    float Dir[3];
    Dir[0] = line.getDirection().x();
    Dir[1] = line.getDirection().y();
    Dir[2] = line.getDirection().z();
    // Вычисляем вспомогательный массив для итераций, показывает октант в который идет распространение
    int Oct[3];
    Oct[0] = ( Dir[0] > 0 ) ? 1 : -1;
    Oct[1] = ( Dir[1] > 0 ) ? 1 : -1;
    Oct[2] = ( Dir[2] > 0 ) ? 1 : -1;
    // Вычисляем добавку, зависящую от направления | НЕ МЕНЯЕТСЯ |
    int Adj[3];
    Adj[0] = ( Dir[0] > 0 ) ? 1 : 0;
    Adj[1] = ( Dir[1] > 0 ) ? 1 : 0;
    Adj[2] = ( Dir[2] > 0 ) ? 1 : 0;
    // Начальная точка для итераций | НЕ МЕНЯЕТСЯ |
    float Bp[3];
    Bp[0] = line.getPosition().x();
    Bp[1] = line.getPosition().y();
    Bp[2] = line.getPosition().z();
    // Найдем индексы ячейки с которой начинаются итерации
    int In[3];
    In[0] = (Bp[0] + Dir[0] * 0.01) / m_xScale;
    In[1] = (Bp[1] + Dir[1] * 0.01) / m_yScale;
    In[2] = (Bp[2] + Dir[2] * 0.01) / m_zScale;
    // Установлю первоначальное значение цвета
    unsigned char color = m_boxNet.getByXyz(In[0], In[1], In[2]);
    // Запишем размеры ячеек в массив для единообразия
    float Size[3];
    Size[0] = m_xScale;
    Size[1] = m_yScale;
    Size[2] = m_zScale;
    // Уравнение на каждой итерации leni = ( ( In[i] + Adj[i] ) * Size[i] - Bp[i] ) / Dir[i];
    // i - компонента (x,y,z)
    // В уравнении меняется только In[i], представлю его в виде y = kx + b
    float k[3];
    k[0] = Size[0] / Dir[0];
    k[1] = Size[1] / Dir[1];
    k[2] = Size[2] / Dir[2];
    float b[3];
    b[0] = (Adj[0] * Size[0] - Bp[0]) / Dir[0];
    //std::cout << b[0] << "\n";
    b[1] = (Adj[1] * Size[1] - Bp[1]) / Dir[1];
    //std::cout << b[1] << "\n";
    b[2] = (Adj[2] * Size[2] - Bp[2]) / Dir[2];
    //std::cout << b[2] << "\n";
    // Крутимся пока не выйдем за пределы основного блока
    float minLen = 0;
    float tempLen;
    float leni;
    int _i;
    while(1) {
//        p++;
        // Найдём минимальную длину !от начальной точки! и соответствующий ей индекс (x, y или z)
        tempLen = minLen;
        minLen = std::numeric_limits<float>::max();
        for (int i = 0; i < 3; i++) {
            leni = k[i] * In[i] + b[i];
            //std::cout << leni << "\n";
            if (leni < minLen) {
                minLen = leni;
                _i = i;
            }
        }
        // Извлекаем цвет
        unsigned char _color = m_boxNet.getByXyz(In[0], In[1], In[2]);
//        unsigned char _color = 12;
        // Сравниваем с предыдущим
        if (color != _color) {
            // В случае несовпадения меняю текущее значение цвета и записываю новый отрезок в line
            line.addSegment(tempLen, color);
            color = _color;
        }
        // Проверяем эту длину на выход за пределы основного блока
        if (minLen > stopLen) {
            line.addSegment(tempLen, color);
            break;
        }
        // Передвигаем индекс In в сторону направления распространения
        In[_i] = In[_i] + Oct[_i];
    }
//    std::cout << "p = " << p << "\n";
}

void CalculationArea::startIterations(Line const & line, float interLayer, double * tk, unsigned char * ck, int & k) {
//    std::cout << "IN NO PARALLEL ITERATE FUNCTION" << std::endl;
//    std::cout << "interlayer = "  << interLayer << std::endl;
//    int itercount = 0;
    // Извлеку максимально возможную длину из line для проверок выхода за пределы блока
    float stopLen = line.getMaxLen() - 0.001;
    // Запишу направление | НЕ МЕНЯЕТСЯ |
    float Dir[3];
    Dir[0] = line.getDirection().x();
    Dir[1] = line.getDirection().y();
    Dir[2] = line.getDirection().z();
    // Вычисляем вспомогательный массив для итераций, показывает октант в который идет распространение
    int Oct[3];
    Oct[0] = ( Dir[0] > 0 ) ? 1 : -1;
    Oct[1] = ( Dir[1] > 0 ) ? 1 : -1;
    Oct[2] = ( Dir[2] > 0 ) ? 1 : -1;
    // Вычисляем добавку, зависящую от направления | НЕ МЕНЯЕТСЯ |
    int Adj[3];
    Adj[0] = ( Dir[0] > 0 ) ? 1 : 0;
    Adj[1] = ( Dir[1] > 0 ) ? 1 : 0;
    Adj[2] = ( Dir[2] > 0 ) ? 1 : 0;
    // Начальная точка для итераций | НЕ МЕНЯЕТСЯ |
    float Bp[3];
    Bp[0] = line.getPosition().x();
    Bp[1] = line.getPosition().y();
    Bp[2] = line.getPosition().z();
    // Найдем индексы ячейки с которой начинаются итерации
    int In[3];
    In[0] = (Bp[0] + Dir[0] * 0.01) / m_xScale;
    In[1] = (Bp[1] + Dir[1] * 0.01) / m_yScale;
    In[2] = (Bp[2] + Dir[2] * 0.01) / m_zScale;
//    std::cout << "Index = { "  << In[0] << " " << In[1] << " " << In[2] << " }" << std::endl;
    // Установлю первоначальное значение цвета
    unsigned char color = m_boxNet.getByXyz(In[0], In[1], In[2]);
    // Запишем размеры ячеек в массив для единообразия
    float Size[3];
    Size[0] = m_xScale;
    Size[1] = m_yScale;
    Size[2] = m_zScale;
    // Уравнение на каждой итерации leni = ( ( In[i] + Adj[i] ) * Size[i] - Bp[i] ) / Dir[i];
    // i - компонента (x,y,z)
    // В уравнении меняется только In[i], представлю его в виде y = kx + b
    float K[3];
    K[0] = Size[0] / Dir[0];
    K[1] = Size[1] / Dir[1];
    K[2] = Size[2] / Dir[2];
    float b[3];
    b[0] = (Adj[0] * Size[0] - Bp[0]) / Dir[0];
    //std::cout << b[0] << "\n";
    b[1] = (Adj[1] * Size[1] - Bp[1]) / Dir[1];
    //std::cout << b[1] << "\n";
    b[2] = (Adj[2] * Size[2] - Bp[2]) / Dir[2];
    //std::cout << b[2] << "\n";
    // Крутимся пока не выйдем за пределы основного блока
    float minLen = 0;
    float tempLen;
    float leni;
    int _i;
//    k = 0;
    float lastLen = - interLayer;
//    std::cout << "IN NO PARALLEL INTERSECT PREV LOOP" << std::endl;
    while(1) {
//        itercount++;
        // Найдём минимальную длину !от начальной точки! и соответствующий ей индекс (x, y или z)
        tempLen = minLen;
        minLen = std::numeric_limits<float>::max();
        for (int i = 0; i < 3; i++) {
            leni = K[i] * In[i] + b[i];
            //std::cout << leni << "\n";
            if (leni < minLen) {
                minLen = leni;
                _i = i;
            }
        }
//        std::cout << "len until next = " << minLen << std::endl;
        // Извлекаем цвет
        unsigned char _color = m_boxNet.getByXyz(In[0], In[1], In[2]);
//        std::cout << "Index = { "  << In[0] << " " << In[1] << " " << In[2] << " } color = " << int(_color) << std::endl;
        // Сравниваем с предыдущим
        if (color != _color) {
            tk[k] = tempLen - lastLen;
            ck[k] = color;
            k++;
            lastLen = tempLen;
            color = _color;
        }
        // Проверяем эту длину на выход за пределы основного блока
        if (minLen > stopLen) {
            tk[k] = line.getMaxLen() - lastLen;
            ck[k] = color;
            k++;
            lastLen = tempLen;
            break;
        }
        // Передвигаем индекс In в сторону направления распространения
        In[_i] = In[_i] + Oct[_i];
    }
//    std::cout << "itercount = " << itercount << std::endl;
}

void CalculationArea::startIterations(Line const & line, float interLayer, int fbIndex, double * tk, unsigned char * ck, int & k) {
    // fbIndex - запрещённый индекс
    // Извлеку максимально возможную длину из line для проверок выхода за пределы блока
    float stopLen = line.getMaxLen() - 0.001;
    // Запишу направление | НЕ МЕНЯЕТСЯ |
    float Dir[3];
    Dir[0] = line.getDirection().x();
    Dir[1] = line.getDirection().y();
    Dir[2] = line.getDirection().z();
    // Вычисляем вспомогательный массив для итераций, показывает октант в который идет распространение
    int Oct[3];
    Oct[0] = ( Dir[0] > 0 ) ? 1 : -1;
    Oct[1] = ( Dir[1] > 0 ) ? 1 : -1;
    Oct[2] = ( Dir[2] > 0 ) ? 1 : -1;
    // Вычисляем добавку, зависящую от направления | НЕ МЕНЯЕТСЯ |
    int Adj[3];
    Adj[0] = ( Dir[0] > 0 ) ? 1 : 0;
    Adj[1] = ( Dir[1] > 0 ) ? 1 : 0;
    Adj[2] = ( Dir[2] > 0 ) ? 1 : 0;
    // Начальная точка для итераций | НЕ МЕНЯЕТСЯ |
    float Bp[3];
    Bp[0] = line.getPosition().x();
    Bp[1] = line.getPosition().y();
    Bp[2] = line.getPosition().z();
    // Найдем индексы ячейки с которой начинаются итерации
    int In[3];
    In[0] = (Bp[0] + Dir[0] * 0.01) / m_xScale;
    In[1] = (Bp[1] + Dir[1] * 0.01) / m_yScale;
    In[2] = (Bp[2] + Dir[2] * 0.01) / m_zScale;
    // Установлю первоначальное значение цвета
    unsigned char color = m_boxNet.getByXyz(In[0], In[1], In[2]);
    // Запишем размеры ячеек в массив для единообразия
    float Size[3];
    Size[0] = m_xScale;
    Size[1] = m_yScale;
    Size[2] = m_zScale;
    // Уравнение на каждой итерации leni = ( ( In[i] + Adj[i] ) * Size[i] - Bp[i] ) / Dir[i];
    // i - компонента (x,y,z)
    // В уравнении меняется только In[i], представлю его в виде y = kx + b
    Dir[fbIndex] = 1;
    float K[3];
    K[0] = Size[0] / Dir[0];
    K[1] = Size[1] / Dir[1];
    K[2] = Size[2] / Dir[2];
    float b[3];
    b[0] = (Adj[0] * Size[0] - Bp[0]) / Dir[0];
    //std::cout << b[0] << "\n";
    b[1] = (Adj[1] * Size[1] - Bp[1]) / Dir[1];
    //std::cout << b[1] << "\n";
    b[2] = (Adj[2] * Size[2] - Bp[2]) / Dir[2];
    //std::cout << b[2] << "\n";
    // Крутимся пока не выйдем за пределы основного блока
    float minLen = 0;
    float tempLen;
    float leni;
    int _i;
//    k = 0;
    float lastLen = - interLayer;
    while(1) {
        // Найдём минимальную длину !от начальной точки! и соответствующий ей индекс (x, y или z)
        tempLen = minLen;
        minLen = std::numeric_limits<float>::max();
        for (int i = 0; i < 3; i++) {
            if (i != fbIndex) {
                leni = K[i] * In[i] + b[i];
                //std::cout << leni << "\n";
                if (leni < minLen) {
                    minLen = leni;
                    _i = i;
                }
            }
        }
        // Извлекаем цвет
        unsigned char _color = m_boxNet.getByXyz(In[0], In[1], In[2]);
        // Сравниваем с предыдущим
        if (color != _color) {
            tk[k] = tempLen - lastLen;
            ck[k] = color;
            k++;
            lastLen = tempLen;
            color = _color;
        }
        // Проверяем эту длину на выход за пределы основного блока
        if (minLen > stopLen) {
            tk[k] = minLen - lastLen;
            ck[k] = color;
            k++;
            break;
        }
        // Передвигаем индекс In в сторону направления распространения
        In[_i] = In[_i] + Oct[_i];
    }
}

void CalculationArea::startParallelIterations(Line & line, float interLayer, int const & index, double * tk, unsigned char * ck, int & k)
{
    // Запишу направление | НЕ МЕНЯЕТСЯ |
    float Dir[3];
    Dir[0] = line.getDirection().x();
    Dir[1] = line.getDirection().y();
    Dir[2] = line.getDirection().z();
    // Вычисляем вспомогательный массив для итераций, показывает октант в который идет распространение
    int Oct = ( Dir[index] > 0 ) ? 1 : -1;
    // Начальная точка для итераций | НЕ МЕНЯЕТСЯ |
    float Bp[3];
    Bp[0] = line.getPosition().x();
    Bp[1] = line.getPosition().y();
    Bp[2] = line.getPosition().z();
    // Найдем индексы ячейки с которой начинаются итерации
    int In[3];
    In[0] = (Bp[0] + Dir[0] * 0.01) / m_xScale;
    In[1] = (Bp[1] + Dir[1] * 0.01) / m_yScale;
    In[2] = (Bp[2] + Dir[2] * 0.01) / m_zScale;
    // Установлю первоначальное значение цвета
    unsigned char color = m_boxNet.getByXyz(In[0], In[1], In[2]);
    // Запишем размеры ячеек в массив для единообразия
    float Size[3];
    Size[0] = m_xScale;
    Size[1] = m_yScale;
    Size[2] = m_zScale;

//    k = 0;
    unsigned char _color;
    float lastLen = - interLayer;
    int size;
    size = line.getMaxLen() / Size[index];
//    if (Oct) {
//        size = m_boxNet.getSize(index) - In[index];
//    } else {
////        size = In[index];
//        size = line.getMaxLen() / Size[index];
//    }
    for (int i = 0; i < size; i++) {
        _color = m_boxNet.getByXyz(In[0], In[1], In[2]);
        if (color != _color) {
            tk[k] = i * Size[index] - lastLen;
            ck[k] = color;
            k++;
            lastLen = i * Size[index];
            color = _color;
        }
        In[index] = In[index] + Oct;
    }
    tk[k] = size * Size[index] - lastLen;
    ck[k] = color;
    k++;
}

int CalculationArea::searchIntersect(Line & line, double * tk, unsigned char * ck, int & k)
{
    // Находим, луч снаружи или внутри основного блока
    // Подготавливаем его
    int err;
    float il;
    if ( hasInsideBox(line.getPosition()) ) {
        err = prepLineIn(line, il);
    } else {
        err = prepLineOut(line, il);
    }
    if (err)
        return err;
    // Оцениваем нет ли параллельности
    if ( line.hasParallel() ) {
        int pFactor = line.getPFactor();
//        std::cout << "PFACTOR == " << pFactor << "\n";
        // Определяем тип параллельности и обрабатываем его
        if (pFactor == 1) { // имеет проекции y и z
            startIterations(line, il, 0, tk, ck, k);
//            std::cout << "HAS Y AND Z\n";
        } else if (pFactor == 2) { // имеет проекции x и z
            startIterations(line, il, 1, tk, ck, k);
        } else if (pFactor == 4) { // имеет проекции x и y
            startIterations(line, il, 2, tk, ck, k);
        } else if (pFactor == 3) { // Имеет только проекцию z
            startParallelIterations(line, il, 2, tk, ck, k);
        } else if (pFactor == 5) { // Имеет только проекцию y
            startParallelIterations(line, il, 1, tk, ck, k);
        } else if (pFactor == 6) { // Имеет только проекцию x
            startParallelIterations(line, il, 0, tk, ck, k);
        }
    } else {
        startIterations(line, il, tk, ck, k);
//        std::cout << "NO PARALLEL"<< "\n";
    }
    return 0;
}

int CalculationArea::costumeIntersect(Line const & line, std::vector <Segment> & segments)
{
    std::vector <Segment> _segments; // Все сегменты ( без объединения )
    for (auto it = m_costume.begin(); it != m_costume.end(); it++) {
        Segment s;
        int err = (*it)->intersect(line, s);
        if (!err) {
            _segments.push_back(s);
//            std::cout << s.pos << "  " << s.end << std::endl;
        }
    }

//    _segments.push_back({10, 20});
//    _segments.push_back({1200, 1800});
//    _segments.push_back({12, 450});
//    _segments.push_back({300, 330});
//    _segments.push_back({480, 1000});

    if ( !_segments.empty() ) {
//        for (auto it = _segments.begin(); it != _segments.end(); it++) {
//            std::cout << (*it).pos << "  " << (*it).end << std::endl;
//        }
        std::sort(_segments.begin(), _segments.end());
//        for (auto it = _segments.begin(); it != _segments.end(); it++) {
//            std::cout << (*it).pos << "  " << (*it).end << std::endl;
//        }
//        std::cout << "--------------" << std::endl;
        float begin = _segments[0].pos;
        float end   = _segments[0].end;
        for (auto it = _segments.begin(); it != _segments.end(); it++) {
            if( (*it).pos > end ) {
                segments.push_back({begin, end});
                begin = (*it).pos;
                end   = (*it).end;
            } else {
                if ( end < (*it).end ) {
                    end = (*it).end;
                }
            }
        }
        segments.push_back({begin, end});
    } else {
        return 1; // пересечений нет
    }

//    for (auto it = segmentBank.begin(); it != segmentBank.end(); it++) {
//        std::cout << (*it).pos << "  " << (*it).end << std::endl;
//    }
    return 0;
}

int CalculationArea::searchIntersectCostume(Line & line, double * tk, unsigned char * ck, int & k)
{
    k = 0;
    // Поиск вектора пересечений с костюмом
    std::vector <Segment> segments;
    int err = costumeIntersect(line, segments);
    if (err)
        return 1; // нет пересечений

    float interLayer;
    for (auto it = segments.begin(); it != segments.end(); it++) {
//        std::cout << "IN SEGMENT LOOP PREV LINE" << std::endl;
        if (!k) {
            interLayer = (*it).pos;
            line.shiftPosition( (*it).pos );
        } else {
            interLayer = (*it).pos - (*(it-1)).end;
            line.shiftPosition( (*it).pos - (*(it-1)).pos );
        }
        line.setMaxLen( (*it).len );
//        std::cout << "IN SEGMENT LOOP POST LINE" << std::endl;
//        std::cout << line << std::endl;
//        std::cout << "interlayer = " << interLayer << std::endl;

        // Оцениваем нет ли параллельности
        if ( line.hasParallel() ) {
            int pFactor = line.getPFactor();
            // Определяем тип параллельности и обрабатываем его
            if (pFactor == 1) { // имеет проекции y и z
                startIterations(line, interLayer, 0, tk, ck, k);
            } else if (pFactor == 2) { // имеет проекции x и z
                startIterations(line, interLayer, 1, tk, ck, k);
            } else if (pFactor == 4) { // имеет проекции x и y
                startIterations(line, interLayer, 2, tk, ck, k);
            } else if (pFactor == 3) { // Имеет только проекцию z
                startParallelIterations(line, interLayer, 2, tk, ck, k);
            } else if (pFactor == 5) { // Имеет только проекцию y
                startParallelIterations(line, interLayer, 1, tk, ck, k);
            } else if (pFactor == 6) { // Имеет только проекцию x
                startParallelIterations(line, interLayer, 0, tk, ck, k);
            }
        } else {
//            std::cout << "IN NOT PARALLEL" << std::endl;
            startIterations(line, interLayer, tk, ck, k);
        }
    }
    return 0;
}

void CalculationArea::setCostume(std::vector <BoundingBox *> costume)
{
    m_costume.swap(costume);
}
