#include "gtest/gtest.h"
#include "calculationarea.hpp"
#include "boxnet.h"
#include "logger.h"
#include "phantom.h"

TEST(calculationArea_test, test_sumple_constructor)
{
    BoxNet box = { 20, 20, 20 };
    CalculationArea area = { box };
}

TEST(calculationArea_test, test_findBeginPoint)
{
    BoxNet box = { 100, 100, 100 };
    for (int iz = 0; iz < 100; iz++) {
        for (int iy = 0; iy < 100; iy++) {
            for (int ix = 0; ix < 100; ix++) {
                int x = ix - 50;
                int y = iy - 50;
                float hyp = sqrt(x*x + y*y);

                if (hyp < 20) {
                    box.setByXyz(ix, iy, iz, 141);
                } else if (hyp < 25 && hyp >= 20) {
                    box.setByXyz(ix, iy, iz, 50);
                }
            }
        }
    }
    Phantom phantom;

    CalculationArea area = { phantom.boxNet() };

//    Line ray = { { -1, -1, -2 }, { 1, 1.3, 1 } };
    Line ray = { { 0, 0, -1 }, { 561, 257, 1740 } };
    int err = area.prepLineOut(ray);
    area.startIterations(ray);
//    Logger::Instance() << "error = " << err;
    Logger::Instance() << ray;
}
