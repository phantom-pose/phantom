#pragma once
#include <functional>

void luDecomp(float ** a,  float ** u, float ** l, int n);
void luSolve(float ** a,  float * b, float * x, int n);
void mnewt(std::function<void(float *, int, float *, float **)>, float * x, int ntrial, int n);
