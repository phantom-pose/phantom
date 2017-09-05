
#include "newt.h"

void luDecomp(float ** a,  float ** u, float ** l, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            u[0][i] = a[0][i];
            float sum = 0;
            for (int k = 0; k < i; k++)
            {
                sum += l[i][k] * u[k][j];
            }
            u[i][j] = (a[i][j] - sum);
            if (i > j)
            {
                l[j][i] = 0;
            }
            else
            {
                sum = 0;
                for (int k = 0; k < i; k++)
                {
                    sum += l[j][k] * u[k][i];
                }
                l[j][i] = (a[j][i] - sum)/u[i][i];
            }
        }
    }
}

void luSolve(float ** a,  float * b, float * x, int n)
{
    float ** u, ** l;
    u = new float*[n];
    l = new float*[n];
    for (int i = 0; i < n; i++)
    {
        u[i] = new float[n];
        l[i] = new float[n];
        for (int j = 0; j < n; j++)
        {
            u[i][j] = 0;
            l[i][j] = 0;
        }
    }
    luDecomp(a, u, l, n);
    float * y = new float[n];
    for (int i = 0; i < n; i++)
    {
        float sum = 0;
        for (int j = 0; j < i; j++)
        {
            sum += l[i][j]*y[j];
        }
        y[i] = (b[i] - sum)/l[i][i];
    }
    for (int i = n - 1; i > -1; i--)
    {
        float sum = 0;
        for (int j = i + 1; j < n; j++)
        {
            sum += u[i][j]*x[j];
        }
        x[i] = (y[i] - sum)/u[i][i];
    }
}

void mnewt(std::function<void(float *, int, float *, float **)> func, float * x, int ntrial, int n)
{
    for (int i = 0; i < ntrial; i++)
    {
        float * fvec = new float[n];
        float ** fjac = new float*[n];
        for (int j = 0; j < n; j++)
        {
            fjac[j] = new float[n];
        }
        float * deltas = new float[n];
        func(x, n, fvec, fjac);
        for (int j = 0; j < n; j++)
        {
            fvec[j] *= -1;
        }
        luSolve(fjac, fvec, deltas, n);
        for (int j = 0; j < n; j++)
        {
            x[j] += deltas[j];
        }
    }
}
