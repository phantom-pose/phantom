#include "gtest/gtest.h"
#include "calculationarea.hpp"
#include "boxnet.h"
#include "logger.h"

TEST(calculationArea_test, test_sumple_constructor)
{
    BoxNet box = { 20, 20, 20 };
    CalculationArea area = { box };
}

TEST(calculationArea_test, test_findBeginPoint)
{
    BoxNet box = { 20, 20, 20 };
    CalculationArea area = { box };

    Point3D <float> beginPoint;
    Line ray = { { -5, 5, 5 }, { 1, 0, 0 } };
    int err = area.findBeginPoint(ray);

}
