#include "joint.h"
#include "newt.h"
#include <cmath>
#include <iostream>
#include <cmath>
#include <boxnet.h>

using namespace std;

void Joint::funcNewt(float * x, float * fvec, float ** fjac, Point3D<float> * point)
{
    float a = x[0];
    float b = x[1];
    float t = x[2];
    auto n_1 = this->getNEndTop(); auto n_2 = m_endPlane2.getN();
    float nx_1 = n_1.getDirection().x();
    float ny_1 = n_1.getDirection().y();
    float nz_1 = n_1.getDirection().z();
    float nx_2 = n_2.getDirection().x();
    float ny_2 = n_2.getDirection().y();
    float nz_2 = n_2.getDirection().z();
    auto e1_1 = m_endPlane1.getE1(); auto e2_1 = m_endPlane1.getE2();
    float e1x_1 = e1_1.getDirection().x() * e1_1.getLength();
    float e1y_1 = e1_1.getDirection().y() * e1_1.getLength();
    float e1z_1 = e1_1.getDirection().z() * e1_1.getLength();
    float e2x_1 = e2_1.getDirection().x() * e2_1.getLength();
    float e2y_1 = e2_1.getDirection().y() * e2_1.getLength();
    float e2z_1 = e2_1.getDirection().z() * e2_1.getLength();
    auto p0_1 = e1_1.getPosition();
    float x0_1 = p0_1.x(); float y0_1 = p0_1.y(); float z0_1 = p0_1.z();
    auto e2_2 = m_endPlane2.getE1(); auto e1_2 = m_endPlane2.getE2();
    float e1x_2 = e1_2.getDirection().x() * e1_2.getLength();
    float e1y_2 = e1_2.getDirection().y() * e1_2.getLength();
    float e1z_2 = e1_2.getDirection().z() * e1_2.getLength();
    float e2x_2 = e2_2.getDirection().x() * e2_2.getLength();
    float e2y_2 = e2_2.getDirection().y() * e2_2.getLength();
    float e2z_2 = e2_2.getDirection().z() * e2_2.getLength();
    auto p0_2 = e1_2.getPosition();
    float x0_2 = p0_2.x(); float y0_2 = p0_2.y(); float z0_2 = p0_2.z();
    double dl_1 = COEF*(
                nx_1*((e1x_2-e1x_1)*a + (e2x_2-e2x_1)*b + (x0_2-x0_1)) +
                ny_1*((e1y_2-e1y_1)*a + (e2y_2-e2y_1)*b + (y0_2-y0_1)) +
                nz_1*((e1z_2-e1z_1)*a + (e2z_2-e2z_1)*b + (z0_2-z0_1))
                );
    double dl_2 = COEF*(
                -nx_2*((e1x_2-e1x_1)*a + (e2x_2-e2x_1)*b + (x0_2-x0_1)) +
                -ny_2*((e1y_2-e1y_1)*a + (e2y_2-e2y_1)*b + (y0_2-y0_1)) +
                -nz_2*((e1z_2-e1z_1)*a + (e2z_2-e2z_1)*b + (z0_2-z0_1))
                );
    double dl_da_1 = COEF*(
                nx_1*(e1x_2-e1x_1) +
                ny_1*(e1y_2-e1y_1) +
                nz_1*(e1z_2-e1z_1)
                );
    double dl_da_2 = COEF*(
                -nx_2*(e1x_2-e1x_1) +
                -ny_2*(e1y_2-e1y_1) +
                -nz_2*(e1z_2-e1z_1)
                );
    double dl_db_1 = COEF*(
                nx_1*(e2x_2-e2x_1) +
                ny_1*(e2y_2-e2y_1) +
                nz_1*(e2z_2-e2z_1)
                );
    double dl_db_2 = COEF*(
                -nx_2*(e2x_2-e2x_1) +
                -ny_2*(e2y_2-e2y_1) +
                -nz_2*(e2z_2-e2z_1)
                );
    fvec[0] = t*t*t*(2*x0_1+2*a*e1x_1+2*b*e2x_1+3*nx_1*dl_1-2*x0_2-2*a*e1x_2-2*b*e2x_2-3*nx_2*dl_2) +
            t*t*(-3*x0_1-3*a*e1x_1-3*b*e2x_1-6*nx_1*dl_1+3*x0_2+3*a*e1x_2+3*b*e2x_2+3*nx_2*dl_2) +
            t*3*nx_1*dl_1 +
            (x0_1+a*e1x_1+b*e2x_1 - point->x());
    fvec[1] = t*t*t*(2*y0_1+2*a*e1y_1+2*b*e2y_1+3*ny_1*dl_1-2*y0_2-2*a*e1y_2-2*b*e2y_2-3*ny_2*dl_2) +
            t*t*(-3*y0_1-3*a*e1y_1-3*b*e2y_1-6*ny_1*dl_1+3*y0_2+3*a*e1y_2+3*b*e2y_2+3*ny_2*dl_2) +
            t*3*ny_1*dl_1 +
            (y0_1+a*e1y_1+b*e2y_1 - point->y());
    fvec[2] = t*t*t*(2*z0_1+2*a*e1z_1+2*b*e2z_1+3*nz_1*dl_1-2*z0_2-2*a*e1z_2-2*b*e2z_2-3*nz_2*dl_2) +
            t*t*(-3*z0_1-3*a*e1z_1-3*b*e2z_1-6*nz_1*dl_1+3*z0_2+3*a*e1z_2+3*b*e2z_2+3*nz_2*dl_2) +
            t*3*nz_1*dl_1 +
            (z0_1+a*e1z_1+b*e2z_1 - point->z());
    fjac[0][0] = t*t*t*(2*e1x_1+3*nx_1*dl_da_1-2*e1x_2-3*nx_2*dl_da_2) +
            t*t*(-3*e1x_1-6*nx_1*dl_da_1+3*e1x_2+3*nx_2*dl_da_2) +
            t*3*nx_1*dl_da_1 +
            e1x_1;
    fjac[0][1] = t*t*t*(2*e2x_1+3*nx_1*dl_db_1-2*e2x_2-3*nx_2*dl_db_2) +
            t*t*(-3*e2x_1-6*nx_1*dl_db_1+3*e2x_2+3*nx_2*dl_db_2) +
            t*3*nx_1*dl_db_1 +
            e2x_1;
    fjac[0][2] = 3*t*t*(2*x0_1+2*a*e1x_1+2*b*e2x_1+3*nx_1*dl_1-2*x0_2-2*a*e1x_2-2*b*e2x_2-3*nx_2*dl_2) +
            2*t*(-3*x0_1-3*a*e1x_1-3*b*e2x_1-6*nx_1*dl_1+3*x0_2+3*a*e1x_2+3*b*e2x_2+3*nx_2*dl_2) +
            3*nx_1*dl_1;
    fjac[1][0] = t*t*t*(2*e1y_1+3*ny_1*dl_da_1-2*e1y_2-3*ny_2*dl_da_2) +
            t*t*(-3*e1y_1-6*ny_1*dl_da_1+3*e1y_2+3*ny_1*dl_da_2) +
            t*3*ny_1*dl_da_1 +
            e1y_1;
    fjac[1][1] = t*t*t*(2*e2y_1+3*ny_1*dl_db_1-2*e2y_2-3*ny_2*dl_db_2) +
            t*t*(-3*e2y_1-6*ny_1*dl_db_1+3*e2y_2+3*ny_2*dl_db_2) +
            t*3*ny_1*dl_db_1 +
            e2y_1;
    fjac[1][2] = 3*t*t*(2*y0_1+2*a*e1y_1+2*b*e2y_1+3*ny_1*dl_1-2*y0_2-2*a*e1y_2-2*b*e2y_2-3*ny_2*dl_2) +
            2*t*(-3*y0_1-3*a*e1y_1-3*b*e2y_1-6*ny_1*dl_1+3*y0_2+3*a*e1y_2+3*b*e2y_2+3*ny_2*dl_2) +
            3*ny_1*dl_1;
    fjac[2][0] = t*t*t*(2*e1z_1+3*nz_1*dl_da_1-2*e1z_2-3*nz_2*dl_da_2) +
            t*t*(-3*e1z_1-6*nz_1*dl_da_1+3*e1z_2+3*nz_2*dl_da_2) +
            t*3*nz_1*dl_da_1 +
            e1z_1;
    fjac[2][1] = t*t*t*(2*e2z_1+3*nz_1*dl_db_1-2*e2z_2-3*nz_2*dl_db_2) +
            t*t*(-3*e2z_1-6*nz_1*dl_db_1+3*e2z_2+3*nz_2*dl_db_2) +
            t*3*nz_1*dl_db_1 +
            e2z_1;
    fjac[2][2] = 3*t*t*(2*z0_1+2*a*e1z_1+2*b*e2z_1+3*nz_1*dl_1-2*z0_2-2*a*e1z_2-2*b*e2z_2-3*nz_2*dl_2) +
            2*t*(-3*z0_1-3*a*e1z_1-3*b*e2z_1-6*nz_1*dl_1+3*z0_2+3*a*e1z_2+3*b*e2z_2+3*nz_2*dl_2) +
            3*nz_1*dl_1;
}

/*!
 * \brief startCoefs - ищет начальное приближение для (альфа, бета, t)
 * \param point
 * \return - начальные (альфа, бета, t)
 */
BezierCoords3D * Joint::startCoefs(Point3D<float> * point)
{
    auto n1 = m_endPlane1.getN();
    auto dir1 = n1.getDirection();
    float A1 = dir1.x(); float B1 = dir1.y(); float C1 = dir1.z();
    auto e1_1 = m_endPlane1.getE1(); auto e2_1 = m_endPlane1.getE2();
    auto ver1 = e1_1.getPosition();
    float D1 = -ver1.x()*A1 - ver1.y()*B1 - ver1.z()*C1;
    float lenn1_s = (A1*point->x() + B1*point->y() + C1*point->z() + D1) / sqrt(A1*A1+B1*B1+C1*C1);
    if (lenn1_s < 0) {
        return new BezierCoords3D(1,1,-1);
    }
    float lenn1 = fabs(lenn1_s);
    Point3D <float> pn1 = {point->x() - A1*lenn1, point->y() - B1*lenn1, point->z() - C1*lenn1};
    Vector3D v1 = {true, ver1, pn1};
    float a1 = (v1.getDirection().x()*e1_1.getDirection().x() +
            v1.getDirection().y()*e1_1.getDirection().y() +
            v1.getDirection().z()*e1_1.getDirection().z()) * v1.getLength()/e1_1.getLength();
    float b1 = (v1.getDirection().x()*e2_1.getDirection().x() +
            v1.getDirection().y()*e2_1.getDirection().y() +
            v1.getDirection().z()*e2_1.getDirection().z()) * v1.getLength()/e2_1.getLength();

    auto n2 = m_endPlane2.getN();
    auto dir2 = n2.getDirection();
    float A2 = dir2.x(); float B2 = dir2.y(); float C2 = dir2.z();
    auto e1_2 = m_endPlane2.getE2(); auto e2_2 = m_endPlane2.getE1();
    auto ver2 = e1_2.getPosition();
    float D2 = -ver2.x()*A2 - ver2.y()*B2 - ver2.z()*C2;
    float lenn2_s = (A2*point->x() + B2*point->y() + C2*point->z() + D2) / sqrt(A2*A2+B2*B2+C2*C2);
    if (lenn2_s < 0) {
        return new BezierCoords3D(1,1,-1);
    }
    float lenn2 = fabs(lenn2_s);
    Point3D <float> pn2 = {point->x() - A2*lenn2, point->y() - B2*lenn2, point->z() - C2*lenn2};
    Vector3D v2 = {true, ver2, pn2};
    float a2 = (v2.getDirection().x()*e1_2.getDirection().x() +
            v2.getDirection().y()*e1_2.getDirection().y() +
            v2.getDirection().z()*e1_2.getDirection().z()) * v2.getLength()/e1_2.getLength();
    float b2 = (v2.getDirection().x()*e2_2.getDirection().x() +
            v2.getDirection().y()*e2_2.getDirection().y() +
            v2.getDirection().z()*e2_2.getDirection().z()) * v2.getLength()/e2_2.getLength();
    return this->choose(a1,a2,b1,b2,lenn1_s,lenn2_s);
}

/*!
 * \brief findAlpha - для точки с известными дек. координатами ищет координаты Безье
 * в деформированном суставе
 * \param point - точка (координаты Безье)
 * \param der - допустимое отклонение дек.координат (сумма квадратов)
 * \return - (альфа, бета, t)
 */
BezierCoords3D * Joint::findAlpha(Point3D<float> * point, float der)
{
    auto startb = this->startCoefs(point);
    float a = startb->alpha();
    float b = startb->beta();
    float t = startb->t();
    //std::cout << a <<" "<< b << " "<< t << "\n";
    if (t < 0)
        return startb;
    std::function<void(float * , int , float * , float ** )> func = [this, point]
            (float * x, int, float * fvec, float ** fjac)
    {
        this->funcNewt(x,fvec,fjac,point);
    };
    float x[3];
    x[0] = a;
    x[1] = b;
    x[2] = t;
    mnewt(func, x, 100, der, 3);
    BezierCoords3D * result = new BezierCoords3D(x[0], x[1], x[2]);
    delete startb;
    return result;
}

/*!
 * \brief findPoint - считает координаты точки из координат Безье в недеформированном суставе
 * \param bc - (альфа, бета, t)
 * \return - точка в декартовых координатах
 */
Point3D<float> * Joint::findPoint(BezierCoords3D * bc)
{
    float a = bc->alpha();
    float b = bc->beta();
    float t = bc->t();
    auto n_1 = this->getNStartTop(); auto n_2 = m_startPlane2.getN();
    float nx_1 = n_1.getDirection().x();
    float ny_1 = n_1.getDirection().y();
    float nz_1 = n_1.getDirection().z();
    float nx_2 = n_2.getDirection().x();
    float ny_2 = n_2.getDirection().y();
    float nz_2 = n_2.getDirection().z();
    auto e1_1 = m_startPlane1.getE1(); auto e2_1 = m_startPlane1.getE2();
    float e1x_1 = e1_1.getDirection().x() * e1_1.getLength();
    float e1y_1 = e1_1.getDirection().y() * e1_1.getLength();
    float e1z_1 = e1_1.getDirection().z() * e1_1.getLength();
    float e2x_1 = e2_1.getDirection().x() * e2_1.getLength();
    float e2y_1 = e2_1.getDirection().y() * e2_1.getLength();
    float e2z_1 = e2_1.getDirection().z() * e2_1.getLength();
    auto p0_1 = e1_1.getPosition();
    float x0_1 = p0_1.x(); float y0_1 = p0_1.y(); float z0_1 = p0_1.z();
    auto e2_2 = m_startPlane2.getE1(); auto e1_2 = m_startPlane2.getE2();
    float e1x_2 = e1_2.getDirection().x() * e1_2.getLength();
    float e1y_2 = e1_2.getDirection().y() * e1_2.getLength();
    float e1z_2 = e1_2.getDirection().z() * e1_2.getLength();
    float e2x_2 = e2_2.getDirection().x() * e2_2.getLength();
    float e2y_2 = e2_2.getDirection().y() * e2_2.getLength();
    float e2z_2 = e2_2.getDirection().z() * e2_2.getLength();
    auto p0_2 = e1_2.getPosition();
    float x0_2 = p0_2.x(); float y0_2 = p0_2.y(); float z0_2 = p0_2.z();
    double dl_1 = COEF*(
                nx_1*((e1x_2-e1x_1)*a + (e2x_2-e2x_1)*b + (x0_2-x0_1)) +
                ny_1*((e1y_2-e1y_1)*a + (e2y_2-e2y_1)*b + (y0_2-y0_1)) +
                nz_1*((e1z_2-e1z_1)*a + (e2z_2-e2z_1)*b + (z0_2-z0_1))
                );
    double dl_2 = COEF*(
                -nx_2*((e1x_2-e1x_1)*a + (e2x_2-e2x_1)*b + (x0_2-x0_1)) +
                -ny_2*((e1y_2-e1y_1)*a + (e2y_2-e2y_1)*b + (y0_2-y0_1)) +
                -nz_2*((e1z_2-e1z_1)*a + (e2z_2-e2z_1)*b + (z0_2-z0_1))
                );
    float x = t*t*t*(2*x0_1+2*a*e1x_1+2*b*e2x_1+3*nx_1*dl_1-2*x0_2-2*a*e1x_2-2*b*e2x_2-3*nx_2*dl_2) +
            t*t*(-3*x0_1-3*a*e1x_1-3*b*e2x_1-6*nx_1*dl_1+3*x0_2+3*a*e1x_2+3*b*e2x_2+3*nx_2*dl_2) +
            t*3*nx_1*dl_1 +
            x0_1+a*e1x_1+b*e2x_1;
    float y = t*t*t*(2*y0_1+2*a*e1y_1+2*b*e2y_1+3*ny_1*dl_1-2*y0_2-2*a*e1y_2-2*b*e2y_2-3*ny_2*dl_2) +
            t*t*(-3*y0_1-3*a*e1y_1-3*b*e2y_1-6*ny_1*dl_1+3*y0_2+3*a*e1y_2+3*b*e2y_2+3*ny_2*dl_2) +
            t*3*ny_1*dl_1 +
            y0_1+a*e1y_1+b*e2y_1;
    float z = t*t*t*(2*z0_1+2*a*e1z_1+2*b*e2z_1+3*nz_1*dl_1-2*z0_2-2*a*e1z_2-2*b*e2z_2-3*nz_2*dl_2) +
            t*t*(-3*z0_1-3*a*e1z_1-3*b*e2z_1-6*nz_1*dl_1+3*z0_2+3*a*e1z_2+3*b*e2z_2+3*nz_2*dl_2) +
            t*3*nz_1*dl_1 +
            z0_1+a*e1z_1+b*e2z_1;
    Point3D<float> * result = new Point3D<float>(x, y, z);
    return result;
}

bool Joint::getStartPoint(Point3D <float> * end, Point3D <float> * start, float der)
{
    //std::cout << m_endPlane1 << m_endPlane2;
    auto bezier = this->findAlpha(end, der);
    auto point = this->findPoint(bezier);
    *start = *point;
    if (bezier->t() < 0 || bezier->t() > 1)
    {
        delete bezier;
        delete point;
        return false;
    }
    delete bezier;
    delete point;
    return true;
}

Point3D <float> RotateX(Point3D <float> rot, Point3D <float>  start, float angle)
{
    return {
        start.x(),
        (start.y()-rot.y())*cos(angle) + (start.z()-rot.z())*sin(angle) + rot.y(),
        (start.z()-rot.z())*cos(angle) - (start.y()-rot.y())*sin(angle) + rot.z()
    };
}

Point3D <float> RotateY(Point3D <float> rot, Point3D <float>  start, float angle)
{
    return {
        (start.x()-rot.x())*cos(angle) + (start.z()-rot.z())*sin(angle) + rot.x(),
        start.y(),
        (start.z()-rot.z())*cos(angle) - (start.x()-rot.x())*sin(angle) + rot.z()
    };
}
