#include "gtest/gtest.h"
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "phantom.h"
#include "calculationarea.hpp"
#include "timetest.h"

TEST(readFile_test, test_readOrgans)
{
    std::string line;

    float * totE1 = new float[100];
    float * totE2 = new float[100];
    float * elE1  = new float[100];
    float * elE2  = new float[100];

    float sigmasTotE1[13];
    float sigmasTotE2[13];
    float sigmasElE1[13];
    float sigmasElE2[13];
    float masses[13] = {1, 12, 14, 16, 23, 24.3, 31, 32, 35.5, 39, 40, 55.8, 127};
    float Na = 6.02;
    float pm[13];
    float density[100];
    int c;

    std::ifstream f1("data/totE1");
    c = 0;
    while (std::getline(f1, line))
    {
        float sigma = stof(line);
        sigmasTotE1[c++] = sigma;
        if (c == 13) { break; }
    }
    f1.close();

    std::ifstream f2("data/totE2");
    c = 0;
    while (std::getline(f2, line))
    {
        float sigma = stof(line);
        sigmasTotE2[c++] = sigma;
        if (c == 13) { break; }
    }
    f2.close();

    std::ifstream f3("data/elE1");
    c = 0;
    while (std::getline(f3, line))
    {
        float sigma = stof(line);
        sigmasElE1[c++] = sigma;
        if (c == 13) { break; }
    }
    f3.close();

    std::ifstream f4("data/elE2");
    c = 0;
    while (std::getline(f4, line))
    {
        float sigma = stof(line);
        sigmasElE2[c++] = sigma;
        if (c == 13) { break; }
    }
    f4.close();

    std::ifstream organsfile("data/AF_organs.dat");
    while (std::getline(organsfile, line))
    {
        std::cout << line << std::endl;
//        std::cout << line.length() << std::endl;
        if (line.length() > 75) {
            std::string id = line.substr(0, 3);
            std::string name = line.substr(6, 48);
            int tiss = stoi(line.substr(55, 2));
            std::string d = line.substr(61, 6);
            density[tiss] = stof(d);
//            std::cout << stoi(id) << "#" << stoi(tissue) << "#" << stof(density) << "#" << std::endl;
        }
    }
    organsfile.close();

//    std::cout << "sigmasTotE1" << std::endl;
//    for (int i = 0; i < 13; i++) {
//        std::cout << sigmasTotE1[i] << std::endl;
//    }
//    std::cout << "sigmasTotE2" << std::endl;
//    for (int i = 0; i < 13; i++) {
//        std::cout << sigmasTotE2[i] << std::endl;
//    }
//    std::cout << "sigmasElE1" << std::endl;
//    for (int i = 0; i < 13; i++) {
//        std::cout << sigmasElE1[i] << std::endl;
//    }
//    std::cout << "sigmasElE2" << std::endl;
//    for (int i = 0; i < 13; i++) {
//        std::cout << sigmasElE2[i] << std::endl;
//    }
//    std::cout << "masses" << std::endl;
//    for (int i = 0; i < 13; i++) {
//        std::cout << masses[i] << std::endl;
//    }





    int tiscount = 0;
    std::ifstream mediafile("data/AF_media.dat");
    int lnum = 0;
    while (std::getline(mediafile, line))
    {
        lnum++;
        if (lnum >= 4) {
//            std::cout << line << std::endl;
            c = 0;
            int   tissue = stoi(line.substr(0, 5));
            float nH     = stof(line.substr(78, 5));
            pm[c++] = nH;
            float nC     = stof(line.substr(84, 5));
            pm[c++] = nC;
            float nN     = stof(line.substr(90, 5));
            pm[c++] = nN;
            float nO     = stof(line.substr(96, 5));
            pm[c++] = nO;
            float nNa    = stof(line.substr(102, 5));
            pm[c++] = nNa;
            float nMg    = stof(line.substr(108, 5));
            pm[c++] = nMg;
            float nP     = stof(line.substr(114, 5));
            pm[c++] = nP;
            float nS     = stof(line.substr(120, 5));
            pm[c++] = nS;
            float nCl    = stof(line.substr(126, 5));
            pm[c++] = nCl;
            float nK     = stof(line.substr(132, 5));
            pm[c++] = nK;
            float nCa    = stof(line.substr(138, 5));
            pm[c++] = nCa;
            float nFe    = stof(line.substr(144, 5));
            pm[c++] = nFe;
            float nI     = stof(line.substr(150, 5));
            pm[c++] = nI;

            double summTotE1 = 0;
            double summTotE2 = 0;
            double summElE1 = 0;
            double summElE2 = 0;
            for (int i = 0; i < 13; i++) {
//                std::cout << i << " " << pm[i] << std::endl;
                summTotE1 = summTotE1 + pm[i] * sigmasTotE1[i] / masses[i];
                summTotE2 += pm[i] * sigmasTotE2[i] / masses[i];
                summElE1 += pm[i] * sigmasElE1[i] / masses[i];
                summElE2 += pm[i] * sigmasElE2[i] / masses[i];
            }

            double sTotE1 = density[tissue] * summTotE1 * Na / 10;
            totE1[tissue] = sTotE1;
            double sTotE2 = density[tissue] * summTotE2 * Na / 10;
            totE2[tissue] = sTotE2;
            double sElE1 = density[tissue] * summElE1 * Na / 10;
            elE1[tissue] = sElE1;
            double sElE2 = density[tissue] * summElE2 * Na / 10;
            elE2[tissue] = sElE2;
            tiscount = tissue;
            //std::cout << tissue << "#" << nH << "#" << nC << "#" << nN << "#" << nO << "#" << nNa << "#"<< nMg << "#" << nP << "#" << nS << "#" << nCl << "#" << nK << "#" << nCa << "#" << nFe << "#" << nI << "#" << std::endl;
        }
    }
    mediafile.close();

    float SmaxTotE1 = 0;
    for (int i = 1; i < tiscount+1; i++) {
        if (SmaxTotE1 < totE1[i]) { SmaxTotE1 = totE1[i]; }
    }
    float SmaxTotE2 = 0;
    for (int i = 1; i < tiscount+1; i++) {
        if (SmaxTotE2 < totE2[i]) { SmaxTotE2 = totE2[i]; }
    }
    float SmaxElE1 = 0;
    for (int i = 1; i < tiscount+1; i++) {
        if (SmaxElE1 < elE1[i]) { SmaxElE1 = elE1[i]; }
    }
    float SmaxElE2 = 0;
    for (int i = 1; i < tiscount+1; i++) {
        if (SmaxElE2 < elE2[i]) { SmaxElE2 = elE2[i]; }
    }

    double ksi = ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );

    Phantom * phantom = new Phantom();

    // Проверяем пересечения
    float x0 = 265.3, y0 = 387.8, z0 = 842, R = 1000;
    float x, y, z;
    float xDir, yDir, zDir;

    CalculationArea area(phantom->boxNet());
    std::vector <BoundingBox *> costume;
    CJsonSerializer::Deserialize(costume, "../sittingCostume.json");
    area.setCostume(costume);
    timetest::initSphere(x0, y0, z0, R);

    double * tk2 = new double [1000];
    unsigned char * ck2 = new unsigned char [1000];
    int k2 = 0;

    clock_t begin2 = clock();
    for (int i = 0; i < 100000; i++) {
        timetest::isotropic(xDir, yDir, zDir);
        k2 = 0;
        Line line2(266, 126, 1323.7, xDir, yDir, zDir);
        area.searchIntersectCostume(line2, tk2, ck2, k2);
    }
    clock_t end2 = clock();
    double elapsed_secs2 = double(end2 - begin2) / CLOCKS_PER_SEC;
    std::cout << "costume seconds = " << elapsed_secs2 << std::endl;

    double * tk1 = new double [1000];
    unsigned char * ck1 = new unsigned char [1000];
    int k1 = 0;


    clock_t begin1 = clock();
    for (int i = 0; i < 100000; i++) {
        timetest::isotropic(xDir, yDir, zDir);
        k1 = 0;
        Line line1(266, 126, 1323.7, xDir, yDir, zDir);
        area.searchIntersect(line1, tk1, ck1, k1);
    }
    clock_t end1 = clock();
    double elapsed_secs1 = double(end1 - begin1) / CLOCKS_PER_SEC;
    std::cout << "Pool seconds = " << elapsed_secs1 << std::endl;

}

void core(Line const & line, double * tk, unsigned char * ck, int const & k, float & pt)
{
}
