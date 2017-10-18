#include "cylinder.h"

Cylinder::Cylinder(float x0, float y0, float z0 , float a, float b, float h)
    : m_pos(x0, y0, z0), m_a(a), m_b(b), m_h(h)
{}

void Cylinder::rotate(RotationMatrix matrix)
{
    m_ex = m_ex + m_pos;
    m_ey = m_ey + m_pos;
    m_ez = m_ez + m_pos;

    matrix.Rotate(m_pos);
    matrix.Rotate(m_ex);
    matrix.Rotate(m_ey);
    matrix.Rotate(m_ez);

    m_ex = m_ex - m_pos;
    m_ey = m_ey - m_pos;
    m_ez = m_ez - m_pos;
}

int Cylinder::intersect(Line const & line, float & t1, float & t2)
{
    Point3D<float> linePos = line.getPosition();
    Point3D<float> lineDir = line.getDirection();

    // Проводим расчёты в с.к. цилиндра
    // Смещаем позицию
    Point3D<float> _linePos = linePos - m_pos;
    // -- Поворачиваем позицию --
    // Формируем матрицу перехода от одного базиса к другому
    float c11 = m_ex.x(), c12 = m_ey.x(), c13 = m_ez.x();
    float c21 = m_ex.y(), c22 = m_ey.y(), c23 = m_ez.y();
    float c31 = m_ex.z(), c32 = m_ey.z(), c33 = m_ez.z();

    // Находим новые координаты
    float x0 = c11 * _linePos.x() + c12 * _linePos.y() + c13 * _linePos.z();
    float y0 = c21 * _linePos.x() + c22 * _linePos.y() + c23 * _linePos.z();
    float z0 = c31 * _linePos.x() + c32 * _linePos.y() + c33 * _linePos.z();

    // -- Поворачиваем направление --
    float xDir = c11 * lineDir.x() + c12 * lineDir.y() + c13 * lineDir.z();
    float yDir = c21 * lineDir.x() + c22 * lineDir.y() + c23 * lineDir.z();
    float zDir = c31 * lineDir.x() + c32 * lineDir.y() + c33 * lineDir.z();

    // Находим положение начальной точки относительно цилиндра
    float s = x0 * x0 / ( m_a * m_a ) + y0 * y0 / ( m_b * m_b );
//    if (s < 1) { return 3; } // точка начала находится внутри цилиндра

    // Находим точку пересечения прямой и цилиндра
    // --Проверяем на параллельность--
    // Вспомогательные переменные
    float k1, k2, k3, k4;
    float a, b, c;
    if (fabs(xDir) > kEps) {
        k1 = x0 / xDir;
        k2 = y0 * xDir - x0 * yDir;
        k3 = (m_b * xDir / m_a) * (m_b * xDir / m_a) + yDir * yDir;
        k4 = ((y0 - m_b) * xDir - x0 * yDir) * ((y0 + m_b) * xDir - x0 * yDir);
        // Определяем коэффициенты квадратного уравнения
        a = 1;
        b = 2 * k1 + 2 * yDir / xDir * k2 / k3;
        c = k1 * k1 + 2 * k1 * yDir * k2 / k3 + k4 / (xDir * xDir * k3);
    } else if (fabs(yDir) > kEps) {
        k1 = y0 / yDir;
        k2 = x0 * yDir - y0 * xDir;
        k3 = (m_a * yDir / m_b) * (m_a * yDir / m_b) + xDir * xDir;
        k4 = ((x0 - m_a) * yDir - y0 * xDir) * ((x0 + m_a) * yDir - y0 * xDir);
        // Определяем коэффициенты квадратного уравнения
        a = 1;
        b = 2 * k1 + 2 * xDir / yDir * k2 / k3;
        c = k1 * k1 + 2 * k1 * xDir * k2 / k3 + k4 / (yDir * yDir * k3);
    } else {
        return 1; // Луч параллелен образующим цилиндра
    }

    int code = quadratic(a, b, c, t1, t2);
    float tt[2];
    tt[0] = t1;
    tt[1] = t2;
    float _tt[2];
    _tt[0] = t1;
    _tt[1] = t2;
    int k = 0; //кол-во корней в _tt
    // Если всё прошло успешно, то в t1 и t2 находятся 2 корня.
    if (!code) {
        // Проводим селекцию: корень должен быть положительным и в пределах высоты цилиндра
        // Подходящие корни помещаю в _tt[2]
        for (int i = 0; i < 2; i++) {
            float z = z0 + zDir * tt[i];
            if (z > 0 && z < m_h && tt[i] >= 0) {
                _tt[k] = tt[i];
                k++;
            }
        }
        if (k == 2) {
            if (_tt[0] > _tt[1]) {
                t1 = _tt[1];
                t2 = _tt[0];
            } else {
                t1 = _tt[0];
                t2 = _tt[1];
            }
            return 0;
        }
        if (!k) { return 4; } // Прямая пересекает цилиндр, но вне его длины
        // Если  k == 1, то ищем дополнительные решения на пересечении с "крышками"
        if (k == 1) {
            t1 = _tt[0];
            float tDown = -z0 / zDir; // t для нижней плоскости
            float xDown = x0 + xDir * tDown;
            float yDown = y0 + yDir * tDown;
            if (hasInsideWalls(xDown, yDown)) {
                t2 = tDown;
                if (t1 > t2) {
                    t1 = tDown;
                    t2 = _tt[0];
                }
            }
            float tTop = (m_h - z0) / zDir; // t для верхней плоскости
            float xTop = x0 + xDir * tTop;
            float yTop = y0 + yDir * tTop;
            if (hasInsideWalls(xTop, yTop)) {
                t2 = tTop;
                if (t1 > t2) {
                    t1 = tTop;
                    t2 = _tt[0];
                }
            }
            return 0;
        }
    } else {
        return 5; // С цилиндром не пересекается либо данные неверны
    }
    return 0;
}

bool Cylinder::hasInsideWalls(float x, float y)
{
    return x * x / ( m_a * m_a ) + y * y / (m_b * m_b ) < 1;
}

int Cylinder::quadratic(float a, float b, float c, float & t1, float & t2)
{
    // Validation
    if (fabs(a) < kEps && fabs(b) < kEps) { return 1; } // Неверные параметры
    if (fabs(a) < kEps) { // Один корень
//        t1 = - c / b;
//        t2 = - c / b;
        return 2;
    }
    float D = b * b - 4 * a * c;
    if (D < 0) { return 3; } // Комплексные решения D < 0

    float A = b / ( 2 * a);
    float B = 4 * a * c / (b * b);
    float C = -1 -sqrt(1 - B);

    t1 = A * C;
    t2 = A * B / C;
    return 0;
}
