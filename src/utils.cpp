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

    int VoxelObjRay(TUnitVec const & uv, double * tk, unsigned char * ck, int & k)
    {
        Line ray = { float(uv.x), float(uv.y), float(uv.z), float(uv.ox), float(uv.oy), float(uv.oz) };
        int err = ca->searchIntersect(ray, tk, ck, k);
        if (!err)
            return err;
        return 0;
    }

}
