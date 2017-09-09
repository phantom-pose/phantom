#include "calculationarea.hpp"

CalculationArea::CalculationArea( BoxNet const & box )
{
    m_boxNet = { box };
    float xLen = m_xScale * m_boxNet.getSizeX();
    float yLen = m_yScale * m_boxNet.getSizeY();
    float zLen = m_zScale * m_boxNet.getSizeZ();
    m_boxSize = { xLen, yLen, zLen };

    // Математически плоскость определяется точкой и нормалью
    // Считаем, что точка расположена снаружи
    // Box состоит из 6ти плоскостей:
    //     { 0, 0, 0 }     { 0, 0, 1 }
    //     { 0, 0, 0 }     { 0, 1, 0 }
    //     { 0, 0, 0 }     { 1, 0, 0 }
    //     { xLen, 0, 0 }  { -1, 0, 0 }
    //     { 0, yLen, 0 }  { 0, -1, 0 }
    //     { 0, 0, zLen }  { 0, 0, -1 }
    m_planes.push_back({ { 0, 0, 0 }, { 0, 0, 1 } });
    m_planes.push_back({ { 0, 0, 0 }, { 0, 1, 0 } });
    m_planes.push_back({ { 0, 0, 0 }, { 1, 0, 0 } });
    m_planes.push_back({ { xLen, 0, 0 }, { -1, 0, 0 } });
    m_planes.push_back({ { 0, yLen, 0 }, { 0, -1, 0 } });
    m_planes.push_back({ { 0, 0, zLen }, { 0, 0, -1 } });
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
int CalculationArea::prepLineOut(Line & line) {
    float len = 0.0f;
    float lens[2];
    int i = 0;
    int parallel = 0;
    int code = 1; // Код результата устанавливаю в значение 1
    for (auto it = m_planes.begin(); it != m_planes.end(); it++) {
        int err = linePlaneIntersect(len, line, *it);
        //std::cout << "err = " << err << "\n";
        // Если хоть какие-нибудь плоскости были пересечены, то функция уже отработала нормально => code = 0
        if (!err) {
            lens[i] = len;
            i++;
            code = 0;
        }
        // Обрабатываю параллельные компоненты
        // Т.е. узнаём пересечения с какими плоскостями никогда не произойдет
        // В данном случае прямая будет параллельна всегда сразу 4м плоскостям,
        // что вычеркивает из рассмотрения 2 компоненты в итерационной процедуре
        if (err == 2 && !parallel) {
            parallel = 1;
            line.setParallel();
        }
    }
    if (!code && i == 2) {
        // На данном этапе в templen и len ближняя и дальняя точки, но неизвестно где какая
        if (lens[1] > lens[0]) {
            line.shiftPosition(lens[0]);
            line.setMaxLen(lens[1]-lens[0]);
        } else {
            line.shiftPosition(lens[1]);
            line.setMaxLen(lens[0]-lens[1]);
        }
        //std::cout << lens[0] << " " << lens[1] << " " << i << "\n";
        return 0;
    }
    return 1;
}

// Для луча внутри основного блока
int CalculationArea::prepLineIn(Line & line) {
    float len = 0.0f;
    int i = 0;
    int parallel = 0;
    int code = 1; // Код результата устанавливаю в значение 1
    for (auto it = m_planes.begin(); it != m_planes.end(); it++) {
        int err = linePlaneIntersect(len, line, *it);
        // Если хоть какие-нибудь плоскости были пересечены, то функция уже отработала нормально => code = 0
        if (!err) {
            code = 0;
            i++;
        }
        // Обрабатываю параллельные компоненты
        // Т.е. узнаём пересечения с какими плоскостями никогда не произойдет
        // В данном случае рассмотрим параллельность двум плоскостям
        // что вычеркивает из рассмотрения 2 компоненты в итерационной процедуре
        if (err == 2 && !parallel) {
            parallel = 1;
            line.setParallel();
        }
    }
    if (!code && i == 1) {
            line.setMaxLen(len);
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

void CalculationArea::startIterations(Line const & line, double * tk, unsigned char * ck, int & k) {
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
    k = 0;
    float lastLen = 0;
    while(1) {
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
            tk[k] = tempLen - lastLen;
            ck[k] = color;
            k++;
            lastLen = tempLen;
            break;
        }
        // Передвигаем индекс In в сторону направления распространения
        In[_i] = In[_i] + Oct[_i];
    }
}

void CalculationArea::startParallelIterations(Line & line, int const & index, double * tk, unsigned char * ck, int & k)
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

    k = 0;
    unsigned char _color;
    float lastLen = 0;
    int size;
    if (Oct) {
        size = m_boxNet.getSize(index) - In[index];
    } else {
        size = In[index];
    }
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
    if ( hasInsideBox(line.getPosition()) ) {
        prepLineIn(line);
    } else {
        prepLineOut(line);
    }
    // Оцениваем нет ли параллельности
    if ( line.hasParallel() ) {
        int index = line.getParallel();
//        startParallelIterations(line, index, tk, ck, k);
        return 1; // параллельно TOTO доделать параллельность
    } else {
        startIterations(line, tk, ck, k);
//        std::cout << "NO PARALLEL"<< "\n";
    }
    return 0;
}
