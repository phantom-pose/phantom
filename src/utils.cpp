#include "utils.h"

namespace utils
{
    CalculationArea * ca;

    int VoxelObjInit(char const * filename )
    {
        BoxNet box = { 299, 137, 348 };
        box.fillFromBin( filename );
        box.segmentation();
        ca = new CalculationArea(box);
        return 0;
    }

    int VoxelObjRay(float x, float y, float z, float ox, float oy, float oz, double * tk, unsigned char * ck, int & k)
    {
        Line ray = { x, y, z, ox, oy, oz };
        int err = ca->searchIntersect(ray, tk, ck, k);
        if (!err)
            return err;
        return 0;
    }

    float dist(float x, float y, float z)
    {
        return sqrt( (x*x*1.875*1.875) + (y*y*1.875*1.875) + (z*z*2.5*2.5) );
    }

}
