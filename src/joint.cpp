#include "joint.h"
#include "newt.h"
#include <cmath>

float constexpr COEF = 0.5;

void f3(float * x, float * fvec, float ** fjac, Plane * plane1, Plane * plane2, Point3D<float> * point)
{
    float a = x[0];
    float b = x[1];
    float t = x[2];
    auto n = plane1->getN(); auto n1 = plane2->getN();
    float nx = n.getDirection().x();
    float ny = n.getDirection().y();
    float nz = n.getDirection().z();
    float nx_1 = n1.getDirection().x();
    float ny_1 = n1.getDirection().y();
    float nz_1 = n1.getDirection().z();
    auto e1 = plane1->getE1(); auto e2 = plane1->getE2();
    float e1x = e1.getDirection().x() * e1.getLength();
    float e1y = e1.getDirection().y() * e1.getLength();
    float e1z = e1.getDirection().z() * e1.getLength();
    float e2x = e2.getDirection().x() * e2.getLength();
    float e2y = e2.getDirection().y() * e2.getLength();
    float e2z = e2.getDirection().z() * e2.getLength();
    auto p0 = e1.getPosition();
    float x0 = p0.x(); float y0 = p0.y(); float z0 = p0.z();
    auto e2_1 = plane2->getE1(); auto e1_1 = plane2->getE2();
    float e1x_1 = e1_1.getDirection().x() * e1_1.getLength();
    float e1y_1 = e1_1.getDirection().y() * e1_1.getLength();
    float e1z_1 = e1_1.getDirection().z() * e1_1.getLength();
    float e2x_1 = e2_1.getDirection().x() * e2_1.getLength();
    float e2y_1 = e2_1.getDirection().y() * e2_1.getLength();
    float e2z_1 = e2_1.getDirection().z() * e2_1.getLength();
    auto p0_1 = e1_1.getPosition();
    float x0_1 = p0_1.x(); float y0_1 = p0_1.y(); float z0_1 = p0_1.z();
    double dl = COEF*(
                nx*((e1x_1-e1x)*a + (e2x_1-e2x)*b + (x0_1-x0)) +
                ny*((e1y_1-e1y)*a + (e2y_1-e2y)*b + (y0_1-y0)) +
                nz*((e1z_1-e1z)*a + (e2z_1-e2z)*b + (z0_1-z0))
                );
    double dl_1 = COEF*(
                -nx_1*((e1x_1-e1x)*a + (e2x_1-e2x)*b + (x0_1-x0)) +
                -ny_1*((e1y_1-e1y)*a + (e2y_1-e2y)*b + (y0_1-y0)) +
                -nz_1*((e1z_1-e1z)*a + (e2z_1-e2z)*b + (z0_1-z0))
                );
    double dl_da = COEF*(
                nx*(e1x_1-e1x) +
                ny*(e1y_1-e1y) +
                nz*(e1z_1-e1z)
                );
    double dl_1_da = COEF*(
                -nx_1*(e1x_1-e1x) +
                -ny_1*(e1y_1-e1y) +
                -nz_1*(e1z_1-e1z)
                );
    double dl_db = COEF*(
                nx*(e2x_1-e2x) +
                ny*(e2y_1-e2y) +
                nz*(e2z_1-e2z)
                );
    double dl_1_db = COEF*(
                -nx_1*(e2x_1-e2x) +
                -ny_1*(e2y_1-e2y) +
                -nz_1*(e2z_1-e2z)
                );
    fvec[0] = t*t*t*(2*x0+2*a*e1x+2*b*e2x+3*nx*dl-2*x0_1-2*a*e1x_1-2*b*e2x_1-3*nx_1*dl_1) +
            t*t*(-3*x0-3*a*e1x-3*b*e2x-6*nx*dl+3*x0_1+3*a*e1x_1+3*b*e2x_1+3*nx_1*dl_1) +
            t*3*nx*dl +
            (x0+a*e1x+b*e2x - point->x());
    fvec[1] = t*t*t*(2*y0+2*a*e1y+2*b*e2y+3*ny*dl-2*y0_1-2*a*e1y_1-2*b*e2y_1-3*ny_1*dl_1) +
            t*t*(-3*y0-3*a*e1y-3*b*e2y-6*ny*dl+3*y0_1+3*a*e1y_1+3*b*e2y_1+3*ny_1*dl_1) +
            t*3*ny*dl +
            (y0+a*e1y+b*e2y - point->y());
    fvec[2] = t*t*t*(2*z0+2*a*e1z+2*b*e2z+3*nz*dl-2*z0_1-2*a*e1z_1-2*b*e2z_1-3*nz_1*dl_1) +
            t*t*(-3*z0-3*a*e1z-3*b*e2z-6*nz*dl+3*z0_1+3*a*e1z_1+3*b*e2z_1+3*nz_1*dl_1) +
            t*3*nz*dl +
            (z0+a*e1z+b*e2z - point->z());
    fjac[0][0] = t*t*t*(2*e1x+3*nx*dl_da-2*e1x_1-3*nx_1*dl_1_da) +
            t*t*(-3*e1x-6*nx*dl_da+3*e1x_1+3*nx_1*dl_1_da) +
            t*3*nx*dl_da +
            e1x;
    fjac[0][1] = t*t*t*(2*e2x+3*nx*dl_db-2*e2x_1-3*nx_1*dl_1_db) +
            t*t*(-3*e2x-6*nx*dl_db+3*e2x_1+3*nx_1*dl_1_db) +
            t*3*nx*dl_db +
            e2x;
    fjac[0][2] = 3*t*t*(2*x0+2*a*e1x+2*b*e2x+3*nx*dl-2*x0_1-2*a*e1x_1-2*b*e2x_1-3*nx_1*dl_1) +
            2*t*(-3*x0-3*a*e1x-3*b*e2x-6*nx*dl+3*x0_1+3*a*e1x_1+3*b*e2x_1+3*nx_1*dl_1) +
            3*nx*dl;
    fjac[1][0] = t*t*t*(2*e1y+3*ny*dl_da-2*e1y_1-3*ny_1*dl_1_da) +
            t*t*(-3*e1y-6*ny*dl_da+3*e1y_1+3*ny_1*dl_1_da) +
            t*3*ny*dl_da +
            e1y;
    fjac[1][1] = t*t*t*(2*e2y+3*ny*dl_db-2*e2y_1-3*ny_1*dl_1_db) +
            t*t*(-3*e2y-6*ny*dl_db+3*e2y_1+3*ny_1*dl_1_db) +
            t*3*ny*dl_db +
            e2y;
    fjac[1][2] = 3*t*t*(2*y0+2*a*e1y+2*b*e2y+3*ny*dl-2*y0_1-2*a*e1y_1-2*b*e2y_1-3*ny_1*dl_1) +
            2*t*(-3*y0-3*a*e1y-3*b*e2y-6*ny*dl+3*y0_1+3*a*e1y_1+3*b*e2y_1+3*ny_1*dl_1) +
            3*ny*dl;
    fjac[2][0] = t*t*t*(2*e1z+3*nz*dl_da-2*e1z_1-3*nz_1*dl_1_da) +
            t*t*(-3*e1z-6*nz*dl_da+3*e1z_1+3*nz_1*dl_1_da) +
            t*3*nz*dl_da +
            e1z;
    fjac[2][1] = t*t*t*(2*e2z+3*nz*dl_db-2*e2z_1-3*nz_1*dl_1_db) +
            t*t*(-3*e2z-6*nz*dl_db+3*e2z_1+3*nz_1*dl_1_db) +
            t*3*nz*dl_db +
            e2z;
    fjac[2][2] = 3*t*t*(2*z0+2*a*e1z+2*b*e2z+3*nz*dl-2*z0_1-2*a*e1z_1-2*b*e2z_1-3*nz_1*dl_1) +
            2*t*(-3*z0-3*a*e1z-3*b*e2z-6*nz*dl+3*z0_1+3*a*e1z_1+3*b*e2z_1+3*nz_1*dl_1) +
            3*nz*dl;
}

class BezierCoords3D
{
public:
    BezierCoords3D(): m_alpha{-0.1}, m_beta{-0.1}, m_t{-1.0}
    {}

    BezierCoords3D(double a,double b, double t): m_alpha{a}, m_beta{b}, m_t{t}
    {}

    ~BezierCoords3D()
    {}

    double const & alpha() const { return m_alpha; }
    double const & beta() const { return m_beta; }
    double const & t() const { return m_t; }
    double m_alpha;
    double m_beta;
    double m_t;
};

BezierCoords3D * StartCoefs(Plane * plane1, Plane * plane2, Point3D<float> * point)
{
    auto n1 = plane1->getN();
    auto dir1 = n1.getDirection();
    float A1 = dir1.x(); float B1 = dir1.y(); float C1 = dir1.z();
    auto e1_1 = plane1->getE1(); auto e1_2 = plane1->getE2();
    auto ver1 = e1_1.getPosition();
    float D1 = -ver1.x()*A1 - ver1.y()*B1 - ver1.z()*C1;
    float lenn1 = fabs((A1*point->x() + B1*point->y() + C1*point->z() + D1)) / sqrt(A1*A1+B1*B1+C1*C1);
    Point3D <float> pn1 = {point->x() - A1*lenn1, point->y() - B1*lenn1, point->z() - C1*lenn1};
    Vector3D v1 = {true, ver1, pn1};
    float a1 = (v1.getDirection().x()*e1_1.getDirection().x() +
            v1.getDirection().y()*e1_1.getDirection().y() +
            v1.getDirection().z()*e1_1.getDirection().z()) * v1.getLength()/e1_1.getLength();
    float b1 = (v1.getDirection().x()*e1_2.getDirection().x() +
            v1.getDirection().y()*e1_2.getDirection().y() +
            v1.getDirection().z()*e1_2.getDirection().z()) * v1.getLength()/e1_2.getLength();

    auto n2 = plane2->getN();
    auto dir2 = n2.getDirection();
    float A2 = dir2.x(); float B2 = dir2.y(); float C2 = dir2.z();
    auto e2_1 = plane2->getE2(); auto e2_2 = plane2->getE1();
    auto ver2 = e2_1.getPosition();
    float D2 = -ver2.x()*A2 - ver2.y()*B2 - ver2.z()*C2;
    float lenn2 = fabs((A2*point->x() + B2*point->y() + C2*point->z() + D2)) / sqrt(A2*A2+B2*B2+C2*C2);
    Point3D <float> pn2 = {point->x() - A2*lenn2, point->y() - B2*lenn2, point->z() - C2*lenn2};
    Vector3D v2 = {true, ver2, pn2};
    float a2 = (v2.getDirection().x()*e2_1.getDirection().x() +
            v2.getDirection().y()*e2_1.getDirection().y() +
            v2.getDirection().z()*e2_1.getDirection().z()) * v2.getLength()/e2_1.getLength();
    float b2 = (v2.getDirection().x()*e2_2.getDirection().x() +
            v2.getDirection().y()*e2_2.getDirection().y() +
            v2.getDirection().z()*e2_2.getDirection().z()) * v2.getLength()/e2_2.getLength();
    float lenn1_s = (A1*point->x() + B1*point->y() + C1*point->z() + D1) / sqrt(A1*A1+B1*B1+C1*C1);
    float lenn2_s = (A2*point->x() + B2*point->y() + C2*point->z() + D2) / sqrt(A2*A2+B2*B2+C2*C2);
    return new BezierCoords3D(a1 > a2 ? a1 : a2, b1 > b2 ? b1 : b2, lenn1_s/(lenn1_s+lenn2_s));
}

/*!
 * \brief FindAlpha - для точки с известными дек. координатами ищет координаты Безье
 * \param plane1 - первая плоскость, альфа вдоль E2, бета вдоль E1
 * \param plane2 - вторая плоскость, альфа вдоль E1, бета вдоль E2
 * \param point - точка (координаты Безье)
 * \param der - допустимое отклонение дек.координат (сумма квадратов)
 * \return - (альфа, бета, t)
 */
BezierCoords3D * FindAlpha(Plane * plane1, Plane * plane2, Point3D<float> * point, float der)
{
    auto startb = StartCoefs(plane1,plane2,point);
    float a = startb->alpha();
    float b = startb->beta();
    float t = startb->t();
    std::function<void(float * , int , float * , float ** )> func = [plane1, plane2, point]
            (float * x, int n, float * fvec, float ** fjac)
    {
        f3(x,fvec,fjac,plane1, plane2,point);
    };
    float x[3];
    x[0] = a;
    x[1] = b;
    x[2] = t;
    mnewt(func, x, 1000, der, 3);
    BezierCoords3D * result = new BezierCoords3D(x[0], x[1], x[2]);
    delete startb;
    return result;
}

/*!
 * \brief FindPoint3 - считает координаты точки из координат Безте
 * \param plane1 - первая плоскость, альфа вдоль E2, бета вдоль E1
 * \param plane2 - вторая плоскость, альфа вдоль E1, бета вдоль E2
 * \param bc - (альфа, бета, t)
 * \return - точка в декартовых координатах
 */
Point3D<float> * FindPoint(Plane * plane1, Plane * plane2, BezierCoords3D * bc)
{
    float a = bc->alpha();
    float b = bc->beta();
    float t = bc->t();
    auto n = plane1->getN(); auto n1 = plane2->getN();
    float nx = n.getDirection().x();
    float ny = n.getDirection().y();
    float nz = n.getDirection().z();
    float nx_1 = n1.getDirection().x();
    float ny_1 = n1.getDirection().y();
    float nz_1 = n1.getDirection().z();
    auto e1 = plane1->getE1(); auto e2 = plane1->getE2();
    float e1x = e1.getDirection().x() * e1.getLength();
    float e1y = e1.getDirection().y() * e1.getLength();
    float e1z = e1.getDirection().z() * e1.getLength();
    float e2x = e2.getDirection().x() * e2.getLength();
    float e2y = e2.getDirection().y() * e2.getLength();
    float e2z = e2.getDirection().z() * e2.getLength();
    auto p0 = e1.getPosition();
    float x0 = p0.x(); float y0 = p0.y(); float z0 = p0.z();
    auto e2_1 = plane2->getE1(); auto e1_1 = plane2->getE2();
    float e1x_1 = e1_1.getDirection().x() * e1_1.getLength();
    float e1y_1 = e1_1.getDirection().y() * e1_1.getLength();
    float e1z_1 = e1_1.getDirection().z() * e1_1.getLength();
    float e2x_1 = e2_1.getDirection().x() * e2_1.getLength();
    float e2y_1 = e2_1.getDirection().y() * e2_1.getLength();
    float e2z_1 = e2_1.getDirection().z() * e2_1.getLength();
    auto p0_1 = e1_1.getPosition();
    float x0_1 = p0_1.x(); float y0_1 = p0_1.y(); float z0_1 = p0_1.z();
    double dl = COEF*(
                nx*((e1x_1-e1x)*a + (e2x_1-e2x)*b + (x0_1-x0)) +
                ny*((e1y_1-e1y)*a + (e2y_1-e2y)*b + (y0_1-y0)) +
                nz*((e1z_1-e1z)*a + (e2z_1-e2z)*b + (z0_1-z0))
                );
    double dl_1 = COEF*(
                -nx_1*((e1x_1-e1x)*a + (e2x_1-e2x)*b + (x0_1-x0)) +
                -ny_1*((e1y_1-e1y)*a + (e2y_1-e2y)*b + (y0_1-y0)) +
                -nz_1*((e1z_1-e1z)*a + (e2z_1-e2z)*b + (z0_1-z0))
                );
    float x = t*t*t*(2*x0+2*a*e1x+2*b*e2x+3*nx*dl-2*x0_1-2*a*e1x_1-2*b*e2x_1-3*nx_1*dl_1) +
            t*t*(-3*x0-3*a*e1x-3*b*e2x-6*nx*dl+3*x0_1+3*a*e1x_1+3*b*e2x_1+3*nx_1*dl_1) +
            t*3*nx*dl +
            x0+a*e1x+b*e2x;
    float y = t*t*t*(2*y0+2*a*e1y+2*b*e2y+3*ny*dl-2*y0_1-2*a*e1y_1-2*b*e2y_1-3*ny_1*dl_1) +
            t*t*(-3*y0-3*a*e1y-3*b*e2y-6*ny*dl+3*y0_1+3*a*e1y_1+3*b*e2y_1+3*ny_1*dl_1) +
            t*3*ny*dl +
            y0+a*e1y+b*e2y;
    float z = t*t*t*(2*z0+2*a*e1z+2*b*e2z+3*nz*dl-2*z0_1-2*a*e1z_1-2*b*e2z_1-3*nz_1*dl_1) +
            t*t*(-3*z0-3*a*e1z-3*b*e2z-6*nz*dl+3*z0_1+3*a*e1z_1+3*b*e2z_1+3*nz_1*dl_1) +
            t*3*nz*dl +
            z0+a*e1z+b*e2z;
    Point3D<float> * result = new Point3D<float>(x, y, z);
    return result;
}

bool Joint::getStartPoint(Point3D <float> * end, Point3D <float> * start, float der)
{
    auto bezier = FindAlpha(m_endPlane1, m_endPlane2, end, der);
    if (bezier->alpha() < 0 || bezier->alpha() > 1
            || bezier->beta() < 0 || bezier->beta() > 1
            || bezier->t() < 0 || bezier->t() > 1)
    {
        delete bezier;
        return false;
    }
    auto point = FindPoint(m_startPlane1, m_startPlane2, bezier);
    delete bezier;
    *start = *point;
    delete point;
    return true;
}
