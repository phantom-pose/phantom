#pragma once

#include <cmath>
#include "line.h"

namespace timetest
{
    int initSphere(float x0, float y0, float z0, float R);
    int sphere(float & x, float & y, float & z, float & xDir, float & yDir, float & zDir);
    int isotropic(float & xDir, float & yDir, float & zDir);
}
