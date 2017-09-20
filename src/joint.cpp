#include "joint.h"
#include "newt.h"
#include <iostream>

float constexpr COEF = 0.5;

void f2yb(float * x, float * fvec, float ** fjac, Plane * plane1, Plane * plane2, Point3D<float> * point)
{
    float a = x[0];
    float t = x[1];
    auto n = plane1->getN(); auto n1 = plane2->getN();
    float nx = n.getDirection().x();
    float nz = n.getDirection().z();
    float nx_1 = n1.getDirection().x();
    float nz_1 = n1.getDirection().z();
    auto e = plane1->getE1();
    float ex = e.getDirection().x() * e.getLength();
    float ez = e.getDirection().z() * e.getLength();
    auto p0 = e.getPosition();
    float x0 = p0.x(); float z0 = p0.z();
    auto e_1 = plane2->getE2();
    float ex_1 = e_1.getDirection().x() * e_1.getLength();
    float ez_1 = e_1.getDirection().z() * e_1.getLength();
    auto p0_1 = e_1.getPosition();
    float x0_1 = p0_1.x(); float z0_1 = p0_1.z();
    double dl = COEF*(nx*(x0_1 + ex_1*a - x0 - ex*a) + nz*(z0_1 + ez_1*a - z0 - ez*a));//нормаль к 1му срезу
    double dl_1 = COEF*(-nx_1*(x0_1 + ex_1*a - x0 - ex*a) - nz_1*(z0_1 + ez_1*a - z0 - ez*a));//нормаль ко второму срезу
    double dl_da = COEF*(nx*(ex_1 - ex) + nz*(ez_1 - ez));//производные к ним во альфе
    double dl_1_da = COEF*(-nx_1*(ex_1 - ez) - nz_1*(ez_1 - ez));
    fvec[0] = t*t*t*(4*x0 + 4*ex*a + 3*nx*dl - 4*x0_1 - 4*ex_1*a -3*nx_1*dl_1) + t*t*(-3*x0 - 3*ex*a - 6*nx*dl + 3*x0_1 + 3*ex_1*a + 3*nx_1*dl_1) + t*(3*dl*nx) + (x0 + ex*a) - point->x();
    fvec[1] = t*t*t*(4*z0 + 4*ez*a + 3*nz*dl - 4*z0_1 - 4*ez_1*a -3*nz_1*dl_1) + t*t*(-3*z0 - 3*ez*a - 6*nz*dl + 3*z0_1 + 3*ez_1*a + 3*nz_1*dl_1) + t*(3*dl*nz) + (z0 + ez*a) - point->z();
    fjac[0][0] = t*t*t*(4*ex + 3*nx*dl_da - 4*ex_1 -3*nx_1*dl_1_da) + t*t*(-3*ex - 6*nx*dl_da + 3*ex_1 + 3*nx_1*dl_1_da) + t*(3*dl_da*nx) + ex;
    fjac[0][1] = 3*t*t*(4*x0 + 4*ex*a + 3*nx*dl - 4*x0_1 - 4*ex_1*a -3*nx_1*dl_1) + 2*t*(-3*x0 - 3*ex*a - 6*nx*dl + 3*x0_1 + 3*ex_1*a + 3*nx_1*dl_1) + (3*dl*nx);
    fjac[1][0] = t*t*t*(4*ez + 3*nz*dl_da - 4*ez_1 -3*nz_1*dl_1_da) + t*t*(-3*ez - 6*nz*dl_da + 3*ez_1 + 3*nz_1*dl_1_da) + t*(3*dl_da*nz) + ez;
    fjac[1][1] = 3*t*t*(4*z0 + 4*ez*a + 3*nz*dl - 4*z0_1 - 4*ez_1*a -3*nz_1*dl_1) + 2*t*(-3*z0 - 3*ez*a - 6*nz*dl + 3*z0_1 + 3*ez_1*a + 3*nz_1*dl_1) + (3*dl*nz);
    std::cout << "[["<< fjac[0][0] << "," << fjac[0][1]  << "]";
    std::cout << "["<< fjac[1][0] << "," << fjac[1][1]  << "]]";
    std::cout << "  ["<< fvec[0] << "," << fvec[1] << "]";
    std::cout << "   x:[" << a << " " << t <<  "]\n";
}

void f2zb(float * x, float * fvec, float ** fjac, Plane * plane1, Plane * plane2, Point3D<float> * point)
{
    float a = x[0];
    float t = x[1];
    auto n = plane1->getN(); auto n1 = plane2->getN();
    float nx = n.getDirection().x();
    float nz = n.getDirection().y(); //!!!
    float nx_1 = n1.getDirection().x();
    float nz_1 = n1.getDirection().y(); //!!!
    auto e = plane1->getE1();
    float ex = e.getDirection().x() * e.getLength();
    float ez = e.getDirection().y() * e.getLength();
    auto p0 = e.getPosition();
    float x0 = p0.x(); float z0 = p0.y();
    auto e_1 = plane2->getE2();
    float ex_1 = e_1.getDirection().x() * e_1.getLength();
    float ez_1 = e_1.getDirection().y() * e_1.getLength();
    auto p0_1 = e_1.getPosition();
    float x0_1 = p0_1.x(); float z0_1 = p0_1.y();
    double dl = COEF*(nx*(x0_1 + ex_1*a - x0 - ex*a) + nz*(z0_1 + ez_1*a - z0 - ez*a));//нормаль к 1му срезу
    double dl_1 = COEF*(-nx_1*(x0_1 + ex_1*a - x0 - ex*a) - nz_1*(z0_1 + ez_1*a - z0 - ez*a));//нормаль ко второму срезу
    double dl_da = COEF*(nx*(ex_1 - ex) + nz*(ez_1 - ez));//производные к ним во альфе
    double dl_1_da = COEF*(-nx_1*(ex_1 - ez) - nz_1*(ez_1 - ez));
    fvec[0] = t*t*t*(4*x0 + 4*ex*a + 3*nx*dl - 4*x0_1 - 4*ex_1*a -3*nx_1*dl_1) + t*t*(-3*x0 - 3*ex*a - 6*nx*dl + 3*x0_1 + 3*ex_1*a + 3*nx_1*dl_1) + t*(3*dl*nx) + (x0 + ex*a) - point->x();
    fvec[1] = t*t*t*(4*z0 + 4*ez*a + 3*nz*dl - 4*z0_1 - 4*ez_1*a -3*nz_1*dl_1) + t*t*(-3*z0 - 3*ez*a - 6*nz*dl + 3*z0_1 + 3*ez_1*a + 3*nz_1*dl_1) + t*(3*dl*nz) + (z0 + ez*a) - point->z();
    fjac[0][0] = t*t*t*(4*ex + 3*nx*dl_da - 4*ex_1 -3*nx_1*dl_1_da) + t*t*(-3*ex - 6*nx*dl_da + 3*ex_1 + 3*nx_1*dl_1_da) + t*(3*dl_da*nx) + ex;
    fjac[0][1] = 3*t*t*(4*x0 + 4*ex*a + 3*nx*dl - 4*x0_1 - 4*ex_1*a -3*nx_1*dl_1) + 2*t*(-3*x0 - 3*ex*a - 6*nx*dl + 3*x0_1 + 3*ex_1*a + 3*nx_1*dl_1) + (3*dl*nx);
    fjac[1][0] = t*t*t*(4*ez + 3*nz*dl_da - 4*ez_1 -3*nz_1*dl_1_da) + t*t*(-3*ez - 6*nz*dl_da + 3*ez_1 + 3*nz_1*dl_1_da) + t*(3*dl_da*nz) + ez;
    fjac[1][1] = 3*t*t*(4*z0 + 4*ez*a + 3*nz*dl - 4*z0_1 - 4*ez_1*a -3*nz_1*dl_1) + 2*t*(-3*z0 - 3*ez*a - 6*nz*dl + 3*z0_1 + 3*ez_1*a + 3*nz_1*dl_1) + (3*dl*nz);
    std::cout << "[["<< fjac[0][0] << "," << fjac[0][1]  << "]";
    std::cout << "["<< fjac[1][0] << "," << fjac[1][1]  << "]]";
    std::cout << "  ["<< fvec[0] << "," << fvec[1] << "]";
    std::cout << "   x:[" << a << " " << t <<  "]\n";
}

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
    fvec[0] = t*t*t*(4*x0+4*a*e1x+4*b*e2x+3*nx*dl-4*x0_1-4*a*e1x_1-4*b*e2x_1-3*nx_1*dl_1) +
            t*t*(-3*x0-3*a*e1x-3*b*e2x-6*nx*dl+3*x0_1+3*a*e1x_1+3*b*e2x_1+3*nx_1*dl_1) +
            t*3*nx*dl +
            (x0+a*e1x+b*e2x - point->x());
    fvec[1] = t*t*t*(4*y0+4*a*e1y+4*b*e2y+3*ny*dl-4*y0_1-4*a*e1y_1-4*b*e2y_1-3*ny_1*dl_1) +
            t*t*(-3*y0-3*a*e1y-3*b*e2y-6*ny*dl+3*y0_1+3*a*e1y_1+3*b*e2y_1+3*ny_1*dl_1) +
            t*3*ny*dl +
            (y0+a*e1y+b*e2y - point->y());
    fvec[2] = t*t*t*(4*z0+4*a*e1z+4*b*e2z+3*nz*dl-4*z0_1-4*a*e1z_1-4*b*e2z_1-3*nz_1*dl_1) +
            t*t*(-3*z0-3*a*e1z-3*b*e2z-6*nz*dl+3*z0_1+3*a*e1z_1+3*b*e2z_1+3*nz_1*dl_1) +
            t*3*nz*dl +
            (z0+a*e1z+b*e2z - point->z());
    fjac[0][0] = t*t*t*(4*e1x+3*nx*dl_da-4*e1x_1-3*nx_1*dl_1_da) +
            t*t*(-3*e1x-6*nx*dl_da+3*e1x_1+3*nx_1*dl_1_da) +
            t*3*nx*dl_da +
            e1x;
    fjac[0][1] = t*t*t*(4*e2x+3*nx*dl_db-4*e2x_1-3*nx_1*dl_1_db) +
            t*t*(-3*e2x-6*nx*dl_db+3*e2x_1+3*nx_1*dl_1_db) +
            t*3*nx*dl_db +
            e2x;
    fjac[0][2] = 3*t*t*(4*x0+4*a*e1x+4*b*e2x+3*nx*dl-4*x0_1-4*a*e1x_1-4*b*e2x_1-3*nx_1*dl_1) +
            2*t*(-3*x0-3*a*e1x-3*b*e2x-6*nx*dl+3*x0_1+3*a*e1x_1+3*b*e2x_1+3*nx_1*dl_1) +
            3*nx*dl;
    fjac[1][0] = t*t*t*(4*e1y+3*ny*dl_da-4*e1y_1-3*ny_1*dl_1_da) +
            t*t*(-3*e1y-6*ny*dl_da+3*e1y_1+3*ny_1*dl_1_da) +
            t*3*ny*dl_da +
            e1y;
    fjac[1][1] = t*t*t*(4*e2y+3*ny*dl_db-4*e2y_1-3*ny_1*dl_1_db) +
            t*t*(-3*e2y-6*ny*dl_db+3*e2y_1+3*ny_1*dl_1_db) +
            t*3*ny*dl_db +
            e2y;
    fjac[1][2] = 3*t*t*(4*y0+4*a*e1y+4*b*e2y+3*ny*dl-4*y0_1-4*a*e1y_1-4*b*e2y_1-3*ny_1*dl_1) +
            2*t*(-3*y0-3*a*e1y-3*b*e2y-6*ny*dl+3*y0_1+3*a*e1y_1+3*b*e2y_1+3*ny_1*dl_1) +
            3*ny*dl;
    fjac[2][0] = t*t*t*(4*e1z+3*nz*dl_da-4*e1z_1-3*nz_1*dl_1_da) +
            t*t*(-3*e1z-6*nz*dl_da+3*e1z_1+3*nz_1*dl_1_da) +
            t*3*nz*dl_da +
            e1z;
    fjac[2][1] = t*t*t*(4*e2z+3*nz*dl_db-4*e2z_1-3*nz_1*dl_1_db) +
            t*t*(-3*e2z-6*nz*dl_db+3*e2z_1+3*nz_1*dl_1_db) +
            t*3*nz*dl_db +
            e2z;
    fjac[2][2] = 3*t*t*(4*z0+4*a*e1z+4*b*e2z+3*nz*dl-4*z0_1-4*a*e1z_1-4*b*e2z_1-3*nz_1*dl_1) +
            2*t*(-3*z0-3*a*e1z-3*b*e2z-6*nz*dl+3*z0_1+3*a*e1z_1+3*b*e2z_1+3*nz_1*dl_1) +
            3*nz*dl;
    /*std::cout << "[["<< fjac[0][0] << "," << fjac[0][1] << "," << fjac[0][2] << "]";
    std::cout << "["<< fjac[1][0] << "," << fjac[1][1] << "," << fjac[1][2] << "]";
    std::cout << "["<< fjac[2][0] << "," << fjac[2][1] << "," << fjac[2][2] << "]]";
    std::cout << "[["<< fvec[0] << "," << fvec[1] << "," << fvec[2] << "]\n";*/
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

BezierCoords3D * FindAlpha3(Plane * plane1, Plane * plane2, Point3D<float> * point)
{
    float a = 0.5;
    float b = 0.5;
    float t = 0.5;
    std::function<void(float * , int , float * , float ** )> func = [plane1, plane2, point]
            (float * x, int n, float * fvec, float ** fjac)
    {
        f3(x,fvec,fjac,plane1, plane2,point);
    };
    float x[3];
    x[0] = a;
    x[1] = b;
    x[2] = t;
    float * fvec = new float[3];
    float ** fjac = new float*[3];
    for (int j = 0; j < 3; j++)
    {
        fjac[j] = new float[3];
    }
    func(x, 3, fvec, fjac);
    if (fjac[0][1] == 0 && fjac[2][1] == 0 && fjac[1][0] == 0 && fjac[1][2] == 0)
    {
        std::function<void(float * , int , float * , float ** )> func2 = [plane1, plane2, point]
                (float * x, int n, float * fvec, float ** fjac)
        {
            f2yb(x,fvec,fjac,plane1, plane2,point);
        };
        float x2[2];
        x2[0] = a;
        x2[1] = t;
        mnewt(func2, x2, 100, 2);
        x[0] = x2[0];
        auto e = plane1->getE2();
        x[1] = (point->y() - e.getPosition().y())/(e.getDirection().y()*e.getLength());
        x[2] = x2[1];
    }
    else if (fjac[1][1] == 0 && fjac[2][1] == 0 && fjac[0][0] == 0 && fjac[0][2] == 0)
    {
        std::function<void(float * , int , float * , float ** )> func2 = [plane1, plane2, point]
                (float * x, int n, float * fvec, float ** fjac)
        {
            f2zb(x,fvec,fjac,plane1, plane2,point);
        };
        float x2[2];
        x2[0] = a;
        x2[1] = t;
        mnewt(func2, x2, 100, 2);
        x[0] = x2[0];
        auto e = plane1->getE2();
        x[1] = (point->z() - e.getPosition().z())/(e.getDirection().z()*e.getLength());
        x[2] = x2[1];
    }
    else
    {
        mnewt(func, x, 100, 3);
    }
    BezierCoords3D * result = new BezierCoords3D(x[0], x[1], x[2]);
    return result;
}

Point3D<float> * FindPoint3(Plane * plane1, Plane * plane2, BezierCoords3D * bc)
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
    float x = t*t*t*(4*x0+4*a*e1x+4*b*e2x+3*nx*dl-4*x0_1-4*a*e1x_1-4*b*e2x_1-3*nx_1*dl_1) +
            t*t*(-3*x0-3*a*e1x-3*b*e2x-6*nx*dl+3*x0_1+3*a*e1x_1+3*b*e2x_1+3*nx_1*dl_1) +
            t*3*nx*dl +
            x0+a*e1x+b*e2x;
    float y = t*t*t*(4*y0+4*a*e1y+4*b*e2y+3*ny*dl-4*y0_1-4*a*e1y_1-4*b*e2y_1-3*ny_1*dl_1) +
            t*t*(-3*y0-3*a*e1y-3*b*e2y-6*ny*dl+3*y0_1+3*a*e1y_1+3*b*e2y_1+3*ny_1*dl_1) +
            t*3*ny*dl +
            y0+a*e1y+b*e2y;
    float z = t*t*t*(4*z0+4*a*e1z+4*b*e2z+3*nz*dl-4*z0_1-4*a*e1z_1-4*b*e2z_1-3*nz_1*dl_1) +
            t*t*(-3*z0-3*a*e1z-3*b*e2z-6*nz*dl+3*z0_1+3*a*e1z_1+3*b*e2z_1+3*nz_1*dl_1) +
            t*3*nz*dl +
            z0+a*e1z+b*e2z;
    Point3D<float> * result = new Point3D<float>(x, y, z);
    return result;
}

bool Joint::getStartPoint(Point3D <float> * end, Point3D <float> * start)
{
    //test
    //auto p = FindPoint3(m_endPlane1, m_endPlane2, new BezierCoords3D(0.6, 0.6, 0.6));
    //auto b = FindAlpha3(m_endPlane1, m_endPlane2, p);
    //std::cout << "TEST " << b->alpha() << " " << b->beta() << " " << b->t()<< std::endl;
    //end test
    auto bezier = FindAlpha3(m_endPlane1, m_endPlane2, end);
    if (bezier->alpha() < 0 || bezier->alpha() > 1
            || bezier->beta() < 0 || bezier->beta() > 1
            || bezier->t() < 0 || bezier->t() > 1)
    {
        std::cout << bezier->alpha() << ' ' << bezier->t() << ' ' << bezier->beta() <<std::endl;
        return false;
    }
    auto point = FindPoint3(m_startPlane1, m_startPlane2, bezier);
    *start = *point;
    return true;
}
