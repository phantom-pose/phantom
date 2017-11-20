#include "gtest/gtest.h"
#include "calculationarea.hpp"
#include "boxnet.h"
#include "logger.h"
#include "phantom.h"
#include "UnitVector.h"
#include "utils.h"
#include "cylinder.h"

float const Eps = 1e-4;

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
//    Logger::Instance() << ray;
//    area.startIterations(ray);
    area.startIterations(ray, 0, tk, ck, k);
//    Logger::Instance() << "error = " << err;
//    Logger::Instance() << ray;
    for (int i = 0; i < k; i++) {
//        std::cout << i << ") len = " << tk[i] << " col = " << int(ck[i]) << "\n";
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

//    Line ray1 = { { -1, 93.75, 20 }, { 1, 0, 0 } }; // Out
    Line ray1 = { { 53 * 1.875, 50 * 1.875, -2.5 }, { 1, 0, 1 } }; // Out
//    Line ray2 = { { 90, 100, 20 }, { 0, -1, 1 } }; // In
    double * tk = new double [10];
    unsigned char * ck = new unsigned char [10];
    int k = 0;

//    utils::VoxelObjInit("data/AF_bin.dat");
//    utils::VoxelObjRay( 0, 0, -1, 561, 257, 1740, tk, ck, k );

//    Cylinder cyl = { 50 * 1.875, 50 * 1.875, 0 , 10 * 1.875, 40 * 1.875, 200 };

//    float t1, t2;

//    int err = cyl.intersect(ray1, t1, t2);
//    std::cout << "ERR = " << err << " t1 = " << t1 << " t2 = " << t2 << "\n";
//    if (!err) {
//        if (t2 < t1) {
//            float t = t1;
//            t1 = t2;
//            t2 = t;
//        }
//        ray1.shiftPosition(t1);
//        ray1.setMaxLen(t2 - t1);
//        area.startParallelIterations(ray1, 0, tk, ck, k);
//    }

//    for (int i = 0; i < k; i++) {
//        std::cout << i << ") len = " << tk[i] << " col = " << int(ck[i]) << "\n";
//    }
}

TEST(calculationArea_test, test_modelIntersect)
{
    BoxNet box = { 100, 100, 100 };
    for (int iz = 0; iz < 100; iz++) {
        for (int iy = 0; iy < 100; iy++) {
            for (int ix = 0; ix < 100; ix++) {
                box.setByXyz(ix, iy, iz, 1);
            }
        }
    }

    // Рисую Layer One
    for (int iz = 15; iz < 55; iz++) {
        for (int iy = 25; iy < 75; iy++) {
            for (int ix = 15; ix < 85; ix++) {
                box.setByXyz(ix, iy, iz, 17);
            }
        }
    }
    // Рисую Layer Two
    for (int iz = 25; iz < 85; iz++) {
        for (int iy = 35; iy < 65; iy++) {
            for (int ix = 25; ix < 35; ix++) {
                box.setByXyz(ix, iy, iz, 77);
            }
        }
    }
    // Рисую Layer Three
    for (int iz = 25; iz < 85; iz++) {
        for (int iy = 35; iy < 65; iy++) {
            for (int ix = 65; ix < 75; ix++) {
                box.setByXyz(ix, iy, iz, 88);
            }
        }
    }
    // Рисую Layer Four
    for (int iz = 30; iz < 50; iz++) {
        for (int iy = 40; iy < 60; iy++) {
            for (int ix = 30; ix < 70; ix++) {
                box.setByXyz(ix, iy, iz, 113);
            }
        }
    }
    std::vector <BoundingBox *> costume;
    costume.push_back(new BoundingBox(18.75, 37.5, 25, 150, 112.5, 125, "alpha"));
    costume.push_back(new BoundingBox(37.5, 56.25, 50, 37.5, 75, 175, "beta"));
    costume.push_back(new BoundingBox(112.5, 56.25, 50, 37.5, 75, 175, "gamma"));

    CalculationArea area(box);
    area.setCostume(costume);
    float f;
    // Parallel x

    // Outside
    Line lineX1(-1, 50 * 1.875, 70 * 2.5, 1, 0, 0);
    std::vector <Segment> segmentsX1;
    area.costumeIntersect(lineX1, segmentsX1);
    f = fabs(segmentsX1[0].pos - (20 * 1.875 + 1));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX1[0].end - (40 * 1.875 + 1));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX1[0].len - 20 * 1.875);
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX1[1].pos - (60 * 1.875 + 1));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX1[1].end - (80 * 1.875 + 1));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX1[1].len - 20 * 1.875);
    EXPECT_LT(f, Eps);

    // Inside
    Line lineX2(30 * 1.875, 50 * 1.875, 70 * 2.5, 1, 0, 0);
    std::vector <Segment> segmentsX2;
    area.costumeIntersect(lineX2, segmentsX2);
    f = fabs(segmentsX2[0].pos - (0));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX2[0].end - (10 * 1.875));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX2[0].len - 10 * 1.875);
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX2[1].pos - (30 * 1.875));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX2[1].end - (50 * 1.875));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsX2[1].len - 20 * 1.875);
    EXPECT_LT(f, Eps);

    // Parallel y

    // Outside
    Line lineY1(30 * 1.875, 500, 30 * 2.5, 0, -1, 0);
    std::vector <Segment> segmentsY1;
    area.costumeIntersect(lineY1, segmentsY1);
    f = fabs(segmentsY1[0].pos - (312.5 + 20 * 1.875));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsY1[0].end - (312.5 + 80 * 1.875));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsY1[0].len - 60 * 1.875);
    EXPECT_LT(f, Eps);

    // Inside
    Line lineY2(30 * 1.875, 70 * 1.875, 30 * 2.5, 0, -1, 0);
    std::vector <Segment> segmentsY2;
    area.costumeIntersect(lineY2, segmentsY2);
    f = fabs(segmentsY2[0].pos - (0));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsY2[0].end - 50 * 1.875);
    EXPECT_LT(f, Eps);
    f = fabs(segmentsY2[0].len - 50 * 1.875);
    EXPECT_LT(f, Eps);

    // Parallel z

    // Outside
    Line lineZ1(30 * 1.875, 50 * 1.875, -3, 0, 0, 1);
    std::vector <Segment> segmentsZ1;
    area.costumeIntersect(lineZ1, segmentsZ1);
    f = fabs(segmentsZ1[0].pos - (10 * 2.5 + 3));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsZ1[0].end - (90 * 2.5 + 3));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsZ1[0].len - 80 * 2.5);
    EXPECT_LT(f, Eps);

    // Inside
    Line lineZ2(30 * 1.875, 50 * 1.875, 70, 0, 0, 1);
    std::vector <Segment> segmentsZ2;
    area.costumeIntersect(lineZ2, segmentsZ2);
    f = fabs(segmentsZ2[0].pos - (0));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsZ2[0].end - 155);
    EXPECT_LT(f, Eps);
    f = fabs(segmentsZ2[0].len - 155);
    EXPECT_LT(f, Eps);

    // Parallel XoY

    // Outside
    Line lineXoY1(-10 * 1.875, 0, 70 * 2.5, 4, 3, 0);
    std::vector <Segment> segmentsXoY1;
    area.costumeIntersect(lineXoY1, segmentsXoY1);
    f = fabs(segmentsXoY1[0].pos - utils::dist(40, 30, 0));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsXoY1[0].end - utils::dist(50, 37.5, 0));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsXoY1[0].len - utils::dist(10, 7.5, 0));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsXoY1[1].pos - utils::dist(70, 52.5, 0));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsXoY1[1].end - utils::dist(90, 67.5, 0));
    EXPECT_LT(f, Eps);
    f = fabs(segmentsXoY1[1].len - utils::dist(20, 15, 0));
    EXPECT_LT(f, Eps);

    // Not parallel
    float d;
    // Outside
    Line line1(-10 * 1.875, -20 * 1.875, -30 * 2.5, 75 * 1.875, 85 * 1.875, 110 * 2.5);
    std::vector <Segment> segments1;
    area.costumeIntersect(line1, segments1);
    d = utils::dist((float)600/17, 40, (float)880/17);
    f = fabs(segments1[0].pos - d);
    std::cout << " pos0 = " << d << std::endl;
    EXPECT_LT(f, Eps);
    d = utils::dist((float)675/11, (float)765/11, (float)90);
    f = fabs(segments1[0].end - d);
    std::cout << " end0 = " << d << std::endl;
    EXPECT_LT(f, Eps);
    d = utils::dist((float)675/11 - (float)600/17, (float)765/11 - (float)40, (float)90 - (float)880/17);
    f = fabs(segments1[0].len - d);
    EXPECT_LT(f, Eps);

    d = utils::dist((float)70, (float)238/3, (float)308/3);
    f = fabs(segments1[1].pos - d);
    std::cout << " pos1 = " << d << std::endl;
    EXPECT_LT(f, Eps);
    d = utils::dist((float)1350/17, (float)90, (float)1980/17);
    f = fabs(segments1[1].end - d);
    std::cout << " end1 = " << d << std::endl;
    EXPECT_LT(f, Eps);
    d = utils::dist((float)1350/17 - (float)70, (float)90 - (float)238/3, (float)1980/17 - (float)308/3);
    f = fabs(segments1[1].len - d);
    EXPECT_LT(f, Eps);

    double * tk = new double [10];
    unsigned char * ck = new unsigned char [10];
    int k = 0;
    float sum = 0;
    area.searchIntersectCostume(line1, tk, ck, k);
    for (int i = 0; i < k; i++) {
        sum += tk[i];
        std::cout << "i = " << i <<" col = " << int(ck[i]) << " len = " << tk[i] << " sum = " << sum << std::endl;
    }
}

TEST(calculationArea_test, test_costumeIntersect)
{
    Phantom phantom;
    CalculationArea area(phantom.boxNet());

    Line line(-0.3, -0.1, 0.73, 0.2951, 0.1376, 0.9455);
    double * tk = new double [1000];
    unsigned char * ck = new unsigned char [1000];
    int k = 0;
    float sum = 0;
    area.searchIntersectCostume(line, tk, ck, k);
    for (int i = 0; i < k; i++) {
        sum += tk[i];
        std::cout << "i = " << i <<" col = " << int(ck[i]) << " len = " << tk[i] << " sum = " << sum << std::endl;
    }

    Line line2(-0.3, -0.1, 0.73, 0.2951, 0.1376, 0.9455);
    std::vector <Segment> segments;
    area.costumeIntersect(line2, segments);
    for (auto it = segments.begin(); it != segments.end(); it++) {
        std::cout << "begin = " << (*it).pos << " end = " << (*it).end << std::endl;
    }

    // TODO Исправления во всех параллелях и случай с заменой цвета и пересечением границы одновременно
}
