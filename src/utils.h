#pragma once

#include "calculationarea.hpp"

namespace utils
{
    int VoxelObjInit( char const * filename );
    int VoxelObjRay(float x, float y, float z, float ox, float oy, float oz, double * tk, unsigned char * ck, int & k);
    float dist(float x, float y, float z);
}
