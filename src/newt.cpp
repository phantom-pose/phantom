#include "newt.h"
#include <iostream>

struct Pair {
    int row;
    int col;

    Pair (int r, int c): row(r), col(c)
    {}
};

Pair findNonZeroCol(float ** a, int m, int n)
{
    for (int j = 0; j < n; j++)
        for (int i=0; i < m; i++)
            if (a[i][j] != 0) return Pair(i,j);
    return Pair(-1,-1);
}

void swap(float ** a, float * b, int n, int row, int col)
{
    float d = a[row][col];
    float t;
    if (row == 0)
        b[row] /= d;
    else {t = b[0]; b[0] = b[row]/d; b[row] = t;}
    for (int j=0; j<n; j++)
    {
        if (row == 0)
            a[row][j] /= d;
        else
        {
            t = a[0][j]; a[0][j] = a[row][j]/d; a[row][j] = t;
        }
    }
}

void simplifyRows(float ** a, float * b, int m, int n, int col)
{
    if (m == 1) return;
    for (int i = 1; i < m; i++)
    {
        float d = a[i][col];
        b[i] -= b[0]*d;
        for (int j=0;j<n;j++) a[i][j] -= a[0][j]*d;
    }
}

float ** subMatrix(float ** a, Pair p, int m, int n)
{
    float ** sm = new float*[m-p.row-1];
    for (int i = 0; i < m-p.row-1; i++)
    {
        sm[i] = new float[n-p.col-1];
        for (int j = 0; j < n - p.col -1; j++)
            sm[i][j] = a[i+p.row+1][j+p.col+1];
    }
    return sm;
}

void triangle(float ** a, float * b, int m, int n)
{
    Pair p = findNonZeroCol(a,m,n);
    if (p.col < 0) return;
    swap(a,b,n,p.row, p.col);
    p.row = 0;
    float * subb = b + 1;
    simplifyRows(a,b,m,n,p.col);

    if (p.row == m || p.col == n) return;
    float ** sm = subMatrix(a,p,m,n);
    triangle(sm, subb, m-1, n-p.col-1);
    for (int i = p.row+1; i < m; i++)
    {
        for (int j = p.col+1; j < n; j++)
            a[i][j] = sm[i-p.row-1][j-p.col-1];
        delete [] sm[i-p.row-1];
    }
    delete [] sm;
}

void zerofy(float ** a, float * b, int m, int n)
{
    if (m == 1 || m > n) return;
    for (int i = m-2; i >= 0; i--)
    {
        b[i] -= b[m-1]*a[i][m-1];
        a[i][m-1] -= a[m-1][m-1]*a[i][m-1];
    }
    zerofy(a,b,m-1,m-1);
}

void solve(float ** a, float * b, int m, int n) {
    triangle(a,b,m,n);
    zerofy(a,b,m,n);
}

void mnewt(std::function<void(float *, int, float *, float **)> func, float * x, int ntrial, int n)
{
    float * deltasCache = new float[n];
    for (int i = 0; i < ntrial; i++)
    {
        float coef = 0.01;
        std::cout << x[0] <<' '<< x[1] << std::endl;
        float * deltas = new float[n];
        float ** fjac = new float*[n];
        for (int j = 0; j < n; j++)
        {
            fjac[j] = new float[n];
        }
        func(x, n, deltas, fjac);
        for (int j = 0; j < n; j++)
        {
            deltas[j] *= -1;
        }
        solve(fjac, deltas, n, n);
        float dsum1 = 0, dsum2 = 0;
        for (int j = 0; j < n; j++)
        {
            dsum1 += deltas[j]*deltas[j];
            dsum2 += deltasCache[j]*deltasCache[j];
        }
        if (dsum1 < 0.0001) {
            delete [] fjac;
            delete [] deltas;
            break;
            //coef = (dsum2/dsum1)/100;
            //std::cout << dsum1 << " " << dsum2 << "[]" << std::endl;
        }
        for (int j = 0; j < n; j++)
        {
            deltasCache[j] = deltas[j];
        }
        for (int j = 0; j < n; j++)
        {
            x[j] += coef*deltas[j];
        }
        for (int j = 0; j < n; j++)
        {
            delete [] fjac[j];
        }
        delete [] fjac;
        delete [] deltas;
    }
    delete [] deltasCache;
}
