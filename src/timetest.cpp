#include "timetest.h"

namespace timetest
{
    float sphereX0;
    float sphereY0;
    float sphereZ0;
    float sphereR;

    int initSphere(float x0, float y0, float z0, float R)
    {
        sphereX0 = x0;
        sphereY0 = y0;
        sphereZ0 = z0;
        sphereR  = R;
    }

    int sphere(float & x, float & y, float & z, float & xDir, float & yDir, float & zDir)
    {
        double ksi1 = ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
        double ksi2 = ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );

        zDir = sphereR * (1 - 2 * ksi1);
        xDir = sphereR * 2 * sqrt( ksi1 - ksi1 * ksi1 ) * cos( 2 * M_PI * ksi2 );
        yDir = sphereR * 2 * sqrt( ksi1 - ksi1 * ksi1 ) * sin( 2 * M_PI * ksi2 );

        z = sphereZ0 - zDir;
        x = sphereX0 - xDir;
        y = sphereY0 - yDir;

        return 0;
    }

    int isotropic(float & xDir, float & yDir, float & zDir)
    {
        double ksi1 = ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
        double ksi2 = ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
        zDir = (1 - 2 * ksi1);
        xDir = 2 * sqrt( ksi1 - ksi1 * ksi1 ) * cos( 2 * M_PI * ksi2 );
        yDir = 2 * sqrt( ksi1 - ksi1 * ksi1 ) * sin( 2 * M_PI * ksi2 );
        return 0;
    }
}
