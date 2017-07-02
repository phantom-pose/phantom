#pragma once

#include <cmath>
#include <point3d.hpp>
#include "logger.h"

using namespace std;

class RotationMatrix
{
public:
    RotationMatrix()
    {
        m_x = 1;
        m_y = 0;
        m_z = 0;
    }

    RotationMatrix(Point3D <float> rotPoint, float phi, float theta, float angle)
    {
        //запоминаем требуемые параметры
        m_phi = phi;
        m_theta = theta;
        m_angle = angle;
        m_rotX = rotPoint.x();
        m_rotY = rotPoint.y();
        m_rotZ = rotPoint.z();
        update();
    }

    RotationMatrix & operator = (RotationMatrix const & obj)
    {
        if ( this == &obj ) return *this;
        m_x = obj.m_x; m_y = obj.m_y; m_z = obj.m_z;
        m_m11 = obj.m_m11; m_m12 = obj.m_m12; m_m13 = obj.m_m13;
        m_m21 = obj.m_m21; m_m22 = obj.m_m22; m_m23 = obj.m_m23;
        m_m31 = obj.m_m31; m_m32 = obj.m_m32; m_m33 = obj.m_m33;
        m_rotX = obj.m_rotX; m_rotY = obj.m_rotY; m_rotZ = obj.m_rotZ;
        m_phi = obj.m_phi; m_theta = obj.m_theta; m_angle = obj.m_angle;
        return *this;
    }

    void update()
    {
        //Устанавливаем параметры вектора вокруг которого производим поворот
        m_x = cos(m_phi) * sin(m_theta) + m_rotX;
        m_y = sin(m_phi) * sin(m_theta) + m_rotY;
        m_z = cos(m_theta) + m_rotZ;

        // Высчитываем коэффициенты для матрицы поворота
        m_m11 = cos(m_angle) + (1 - cos(m_angle)) * m_x * m_x;
        m_m12 = (1 - cos(m_angle)) * m_x * m_y - m_z * sin(m_angle);
        m_m13 = (1 - cos(m_angle)) * m_x * m_z + m_y * sin(m_angle);
        m_m21 = (1 - cos(m_angle)) * m_x * m_y + m_z * sin(m_angle);
        m_m22 = cos(m_angle) + (1 - cos(m_angle)) * m_y * m_y;
        m_m23 = (1 - cos(m_angle)) * m_y * m_z - m_x * sin(m_angle);
        m_m31 = (1 - cos(m_angle)) * m_x * m_z - m_y * sin(m_angle);
        m_m32 = (1 - cos(m_angle)) * m_z * m_y + m_x * sin(m_angle);
        m_m33 = cos(m_angle) + (1 - cos(m_angle)) * m_z * m_z;
    }

    void multiply(RotationMatrix const & obj)
    {
        float c11 = m_m11 * obj.m_m11 + m_m12 * obj.m_m21 + m_m13 * obj.m_m31;
        float c12 = m_m11 * obj.m_m12 + m_m12 * obj.m_m22 + m_m13 * obj.m_m32;
        float c13 = m_m11 * obj.m_m13 + m_m12 * obj.m_m23 + m_m13 * obj.m_m33;

        float c21 = m_m21 * obj.m_m11 + m_m22 * obj.m_m21 + m_m23 * obj.m_m31;
        float c22 = m_m21 * obj.m_m12 + m_m22 * obj.m_m22 + m_m23 * obj.m_m32;
        float c23 = m_m21 * obj.m_m13 + m_m22 * obj.m_m23 + m_m23 * obj.m_m33;

        float c31 = m_m31 * obj.m_m11 + m_m32 * obj.m_m21 + m_m33 * obj.m_m31;
        float c32 = m_m31 * obj.m_m12 + m_m32 * obj.m_m22 + m_m33 * obj.m_m32;
        float c33 = m_m31 * obj.m_m13 + m_m32 * obj.m_m23 + m_m33 * obj.m_m33;

        m_m11 = c11; m_m12 = c12; m_m13 = c13;
        m_m21 = c21; m_m22 = c22; m_m23 = c23;
        m_m31 = c31; m_m32 = c32; m_m33 = c33;
    }

    void Rotate(float & x, float & y, float & z)
    {
        m_bx = x * m_m11 + y * m_m12 + z * m_m13;
        m_by = x * m_m21 + y * m_m22 + z * m_m23;
        m_bz = x * m_m31 + y * m_m32 + z * m_m33;
        x = m_bx;
        y = m_by;
        z = m_bz;
    }

    void setRotPoint(Point3D <float> const & rotP)
    {
        m_rotX = rotP.x();
        m_rotY = rotP.y();
        m_rotZ = rotP.z();
        update();
    }

    void setAngle(float const & angle)
    {
        m_angle = angle;
        update();
    }

    void setVector(float const & phi, float const & theta)
    {
        m_phi = phi;
        m_theta = theta;
        update();
    }

    void Rotate( Point3D <float> & crd)
    {
        float x = crd.x();
        float y = crd.y();
        float z = crd.z();

        Rotate(x, y, z);

        crd.setValue(x, y, z);
    }

private:
    float m_x, m_y, m_z;
    float m_bx, m_by, m_bz;
    float m_m11, m_m12, m_m13, m_m21, m_m22, m_m23, m_m31, m_m32, m_m33;
    float m_rotX, m_rotY, m_rotZ;
    float m_phi, m_theta;
    float m_angle;
};
