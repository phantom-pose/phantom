#include "calculationarea.hpp"

CalculationArea::CalculationArea( BoxNet const & box )
{
    m_boxNet = { box };
    float xLen = m_xScale * m_boxNet.getSizeX();
    float yLen = m_yScale * m_boxNet.getSizeY();
    float zLen = m_zScale * m_boxNet.getSizeZ();
    m_boxSize = { xLen, yLen, zLen };
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

int CalculationArea::findBeginPoint(Vector3D const & segment, Point3D <float> & pt) {
    // Для начала необходимо отобрать 3 возможные плоскости для первого пересечения
    // математически плоскость определяется точкой и нормалью
    // Считаем, что точка расположена снаружи
    // Box состоит из 6ти плоскостей:
    //     { 0, 0, 0 }     { 0, 0, 1 }
    //     { 0, 0, 0 }     { 0, 1, 0 }
    //     { 0, 0, 0 }     { 1, 0, 0 }
    //     { xLen, 0, 0 }  { -1, 0, 0 }
    //     { 0, yLen, 0 }  { 0, -1, 0 }
    //     { 0, 0, zLen }  { 0, 0, -1 }
    std::vector <Vector3D> planes;
    planes.push_back({ { 0, 0, 0 }, { 0, 0, 1 } });
    planes.push_back({ { 0, 0, 0 }, { 0, 1, 0 } });
    planes.push_back({ { 0, 0, 0 }, { 1, 0, 0 } });
    planes.push_back({ { m_boxSize.x(), 0, 0 }, { -1, 0, 0 } });
    planes.push_back({ { 0, m_boxSize.y(), 0 }, { 0, -1, 0 } });
    planes.push_back({ { 0, 0, m_boxSize.z() }, { 0, 0, -1 } });

    float len = std::numeric_limits<float>::max();
    for (auto it = planes.begin(); it != planes.end(); it++) {
        int err = linePlaneIntersect(len, segment, *it, pt);
        if (!err) {
            return 0;
        }
    }
    return 1;
}

int CalculationArea::linePlaneIntersect(float & len, Vector3D const & segment, Vector3D const & plane, Point3D <float> & pt) {

    Point3D <float> u = segment.getDirection();
    Point3D <float> w = segment.getPosition() - plane.getPosition();
    float D = plane.getDirection() * u;
    float N = -(plane.getDirection() * w);

    if (fabs(D) < kEps) {
        if (fabs(N) < kEps)
            return 2; // прямая принадлежит плоскости
        else
            return 1; // Не пересекает
    }
    float s = N / D;
    if (s < 0)
        return 0;
    Point3D <float> point = segment.getPosition() + u * s;
    Point3D <float> checkdot = point + plane.getDirection();
    if ( hasInsideBox(checkdot) ) {
        if (len > s) {
            len = s;
            pt = point;
            return 0; // всё успешно, плоскость найдена
        }
        return 3; // плоскость найдена, но дальняя
    }
    return 4; // плоскость пересекает, но за пределами коробки
}

void CalculationArea::startIterations(Line & line) {

    // Установлю первоначальное значение цвета (невозможный цвет)
    unsigned char color = 142;
    float Dir[3];
    int In[3];
    float Bp[3];
    float Size[3];
    int Oct[3];
    int Adj[3];
    // Запишу направление в массив, оно не меняется | НЕ МЕНЯЕТСЯ |
    Dir[0] = line.getDirection().x();
    Dir[1] = line.getDirection().y();
    Dir[2] = line.getDirection().z();
    // Найдем октант в который распространяется луч | НЕ МЕНЯЕТСЯ |
    Oct[0] = ( Dir[0] > 0 ) ? 1 : -1;
    Oct[1] = ( Dir[1] > 0 ) ? 1 : -1;
    Oct[2] = ( Dir[2] > 0 ) ? 1 : -1;
    // Вычисляем добавку, зависящую от направления | НЕ МЕНЯЕТСЯ |
    Adj[0] = ( Dir[0] > 0 ) ? 1 : 0;
    Adj[1] = ( Dir[1] > 0 ) ? 1 : 0;
    Adj[2] = ( Dir[2] > 0 ) ? 1 : 0;
    // Начальная точка для итераций | НЕ МЕНЯЕТСЯ |
    Bp[0] = line.getPosition().x();
    Bp[1] = line.getPosition().y();
    Bp[2] = line.getPosition().z();
    // Найдем индексы ячейки с которой начинаются итерации
    In[0] = (Bp[0] + Oct[0] * 0.001) / m_xScale;
    In[1] = (Bp[1] + Oct[1] * 0.001) / m_yScale;
    In[2] = (Bp[2] + Oct[2] * 0.001) / m_zScale;
    // Запишем размеры ячеек в массив для единообразия
    Size[0] = m_xScale;
    Size[1] = m_yScale;
    Size[2] = m_zScale;

    // Крутимся пока не выйдем за пределы основного блока
    while(1) {
        // Найдём минимальную длину !от начальной точки! и соответствующий ей индекс (x, y или z)
        float minLen = std::numeric_limits<float>::max();
        float leni;
        int _i;
        for (int i = 0; i < 3; i++) {
            leni = ( ( In[i] + Adj[i] ) * Size[i] - Bp[i] ) * Oct[i] / Dir[i];
            if (leni < minLen) {
                minLen = leni;
                _i = i;
            }
        }
        // Проверяем эту длину на выход за пределы основного блока

        // Извлекаем цвет
        unsigned char _color = m_boxNet.getByXyz(In[0], In[1], In[2]);
        // Сравниваем с предыдущим
        if (color != _color) {
            color = _color;
        }
        // Передвигаем индекс In в сторону направления распространения
        In[_i] = In[_i] * Oct[_i];
        //
    }
}
