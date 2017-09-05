#include "joint.h"
#include "newt.h"

float constexpr COEF = 0.5;

void f3(float * x, float * fvec, float ** fjac, Plane * plane1, Plane * plane2, Point3D<float> * point)
{
    float a = x[0];
    float b = x[1];
    float t = x[2];
    auto n = plane1->getN(); auto n1 = plane2->getN();
    float nx = -(n.getDirection().x());
    float ny = -(n.getDirection().y());
    float nz = -(n.getDirection().z());
    float nx_1 = n1.getDirection().x();
    float ny_1 = n1.getDirection().y();
    float nz_1 = n1.getDirection().z();
    auto e1 = plane1->getE1(); auto e2 = plane1->getE2();
    float e1x = e1.getDirection().x();
    float e1y = e1.getDirection().y();
    float e1z = e1.getDirection().z();
    float e2x = e2.getDirection().x();
    float e2y = e2.getDirection().y();
    float e2z = e2.getDirection().z();
    auto p0 = e1.getPosition();
    float x0 = p0.x(); float y0 = p0.y(); float z0 = p0.z();
    auto e1_1 = plane1->getE1(); auto e2_1 = plane1->getE2();
    float e1x_1 = e1_1.getDirection().x();
    float e1y_1 = e1_1.getDirection().y();
    float e1z_1 = e1_1.getDirection().z();
    float e2x_1 = e2_1.getDirection().x();
    float e2y_1 = e2_1.getDirection().y();
    float e2z_1 = e2_1.getDirection().z();
    auto p0_1 = e1.getPosition();
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
            t*t*(-3*x0-3*a*e1x-3*e2x-6*nx*dl-6*x0_1-6*a*e1x_1-6*b*e2x_1-6*nx_1*dl_1) +
            t*3*nx*dl +
            (x0+a*e1x+b*e2x - point->x());
    fvec[1] = t*t*t*(2*y0+2*a*e1y+2*b*e2y+3*ny*dl-2*y0_1-2*a*e1y_1-2*b*e2y_1-3*ny_1*dl_1) +
            t*t*(-3*y0-3*a*e1y-3*e2y-6*ny*dl-6*y0_1-6*a*e1y_1-6*b*e2y_1-6*ny_1*dl_1) +
            t*3*ny*dl +
            (y0+a*e1y+b*e2y - point->y());
    fvec[2] = t*t*t*(2*z0+2*a*e1z+2*b*e2z+3*nz*dl-2*z0_1-2*a*e1z_1-2*b*e2z_1-3*nz_1*dl_1) +
            t*t*(-3*z0-3*a*e1z-3*e2z-6*nz*dl-6*z0_1-6*a*e1z_1-6*b*e2z_1-6*nz_1*dl_1) +
            t*3*nz*dl +
            (z0+a*e1z+b*e2z - point->z());
    fjac[0][0] = t*t*t*(2*e1x+3*nx*dl_da-2*e1x_1-3*nx_1*dl_1_da) +
            t*t*(-3*e1x-6*nx*dl_da-6*e1x_1-6*nx_1*dl_1_da) +
            t*3*nx*dl_da +
            e1x;
    fjac[0][1] = t*t*t*(2*e2x+3*nx*dl_db-2*e2x_1-3*nx_1*dl_1_db) +
            t*t*(-3*e2x-6*nx*dl_db-6*e2x_1-6*nx_1*dl_1_db) +
            t*3*nx*dl_db +
            e2x;
    fjac[0][2] = 3*t*t*(2*x0+2*a*e1x+2*b*e2x+3*nx*dl-2*x0_1-2*a*e1x_1-2*b*e2x_1-3*nx_1*dl_1) +
            2*t*(-3*x0-3*a*e1x-3*e2x-6*nx*dl-6*x0_1-6*a*e1x_1-6*b*e2x_1-6*nx_1*dl_1) +
            3*nx*dl;
    fjac[1][0] = t*t*t*(2*e1y+3*ny*dl_da-2*e1y_1-3*ny_1*dl_1_da) +
            t*t*(-3*e1y-6*ny*dl_da-6*e1y_1-6*ny_1*dl_1_da) +
            t*3*ny*dl_da +
            e1y;
    fjac[1][1] = t*t*t*(2*e2y+3*ny*dl_db-2*e2y_1-3*ny_1*dl_1_db) +
            t*t*(-3*e2y-6*ny*dl_db-6*e2y_1-6*ny_1*dl_1_db) +
            t*3*ny*dl_db +
            e2y;
    fjac[1][2] = 3*t*t*(2*y0+2*a*e1y+2*b*e2y+3*ny*dl-2*y0_1-2*a*e1y_1-2*b*e2y_1-3*ny_1*dl_1) +
            2*t*(-3*y0-3*a*e1y-3*e2y-6*ny*dl-6*y0_1-6*a*e1y_1-6*b*e2y_1-6*ny_1*dl_1) +
            3*ny*dl;
    fjac[2][0] = t*t*t*(2*e1z+3*nz*dl_da-2*e1z_1-3*nz_1*dl_1_da) +
            t*t*(-3*e1z-6*nz*dl_da-6*e1z_1-6*nz_1*dl_1_da) +
            t*3*nz*dl_da +
            e1z;
    fjac[2][1] = t*t*t*(2*e2z+3*nz*dl_db-2*e2z_1-3*nz_1*dl_1_db) +
            t*t*(-3*e2z-6*nz*dl_db-6*e2z_1-6*nz_1*dl_1_db) +
            t*3*nz*dl_db +
            e2z;
    fjac[2][2] = 3*t*t*(2*z0+2*a*e1z+2*b*e2z+3*nz*dl-2*z0_1-2*a*e1z_1-2*b*e2z_1-3*nz_1*dl_1) +
            2*t*(-3*z0-3*a*e1z-3*e2z-6*nz*dl-6*z0_1-6*a*e1z_1-6*b*e2z_1-6*nz_1*dl_1) +
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
    mnewt(func, x, 100, 2);
    BezierCoords3D * result = new BezierCoords3D(x[0], x[1], x[2]);
    return result;
}

Joint::~Joint()
{
    delete m_startPlane1;
    delete m_startPlane2;
    delete m_endPlane1;
    delete m_endPlane2;
}

bool Joint::getStartPoint(Point3D <float> * end, Point3D <float> * start)
{
    return true;
}
