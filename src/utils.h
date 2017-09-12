#pragma once

#include "calculationarea.hpp"
#include "UnitVector.h"

namespace utils
{
    int VoxelObjInit( char const * filename );
    int VoxelObjRay(TUnitVec const & uv, double * tk, unsigned char * ck, int & k);
}
