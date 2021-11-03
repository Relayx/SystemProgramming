#include "equation.h"

//-------------------------------------------------------------
static const double EPS = 10e-9;

typedef unsigned short bool;
#define TRUE 1;
#define FALSE 0;

static bool isZeroDouble(double d);
//-------------------------------------------------------------

Roots QuadraticEquation_ApiV1_0(double a, double b, double c, 
                                double* x1, double* x2)
{
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    assert(x1 != NULL);
    assert(x2 != NULL);

    assert(x1 != x2);

    if (a == 0)
        return LinearEquation_ApiV1_0(b, c, x1);
    
    double d = (b * b) - (4 * a * c);

    if (d < 0)
        return NO_ROOTS;

    double cachedSqrt = sqrt(d);

    *x1 = (-b + cachedSqrt) / (2 * a);
    *x2 = (-b - cachedSqrt) / (2 * a);

    return TWO_ROOTS;
}

Roots LinearEquation_ApiV1_0(double k, double b, double* x)
{
    assert(isfinite(k));
    assert(isfinite(b));

    assert(x != NULL);

    // Я злой и страшный серый walk
    // Я в поросятах знаю talk

    if (k == 0)
    {
        if (b == 0)
            return INF_ROOTS;
        else 
            return NO_ROOTS;
    }

    *x = -b / k;
    
    return ONE_ROOT;
}

static bool isZeroDouble(double d)
{
    assert(isfinite(d));

    if (fabs(d) < EPS)
        return TRUE;
    return FALSE;
}

/* (c)Relayx (Nikita Zvezdin)
* Free for modification and distribution with the indication of the author
* GitHub: https://github.com/Relayx
*/