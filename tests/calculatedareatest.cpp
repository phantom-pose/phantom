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

    CalculationArea area = { box };

    double * tk = new double [1500];
    unsigned char * ck = new unsigned char [1500];
    int k = 0;

    Line ray = { { -1, -1, -2 }, { 1, 1.3, 1 } };
//    Line ray = { { 0, 0, -1 }, { 561, 257, 1740 } };
    int err = area.prepLineOut(ray);
    Logger::Instance() << ray;
//    area.startIterations(ray);
    area.startIterations(ray, tk, ck, k);
//    Logger::Instance() << "error = " << err;
    Logger::Instance() << ray;
    for (int i = 0; i < k; i++) {
        std::cout << i << ") len = " << tk[i] << " col = " << int(ck[i]) << "\n";
    }
}

TEST(calculationArea_test, test_searchIntersect)
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
    CalculationArea area = { box };

    Line ray1 = { { 90, -1, 20 }, { 0, 1, 0 } }; // Out
    Line ray2 = { { 90, 100, 20 }, { 0.01, -1, 1 } }; // In
    double * tk = new double [10];
    unsigned char * ck = new unsigned char [10];
    int k = 0;
//    area.searchIntersect(ray1, tk, ck, k);
    area.searchIntersect(ray2, tk, ck, k);
//    Logger::Instance() << ray1;
    Logger::Instance() << ray2;
    for (int i = 0; i < k; i++) {
        std::cout << i << ") len = " << tk[i] << " col = " << int(ck[i]) << "\n";
    }
}
