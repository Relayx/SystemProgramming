#include <stdio.h>

#include "../Equation/equation.h"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN  "\033[1;32m"

static int TEST_NUM;
static Equation api;

#define TEST_PACK(PACK) \
    printf(PACK);       \
    TEST_NUM = 1;

void Test(double a, double b, double c, int out, double _x1, double _x2)
{
    printf("[%d]", TEST_NUM);

    double x1 = NAN, x2 = NAN;
    int numRoots = api->QuadraticEquation(a, b, c, &x1, &x2);
    if (    numRoots != out ||
            ( x1 != _x1 && !isnan(x1) && !isnan(_x1) ) ||
            ( x2 != _x2 && !isnan(x2) && !isnan(_x2)) ||
            (isnan(x2) != isnan(_x2)) ||
            (isnan(x1) != isnan(_x1)) )
    {
        printf("%s[FAILED]%s", RED, RESET);
        printf("\nExpected numRoots = %d, x1 = %lg, x2 = %lg \nBut result: numRoots = %d x1 = %lg x2 = %lg\n",
                             out,     _x1,      _x2,                     numRoots, x1, x2        );
        exit(EXIT_FAILURE);
    }
    else
        printf("%s[PASSED]%s\n", GREEN, RESET);
    TEST_NUM++;
}

int main()
{
    if((api = EquationInit()) == NULL)
    {
        assert(!"Fatal Error! Equation lib was not found");
        return 0;
    }

    TEST_PACK("Linear equat tests\n");
    Test(0, 2, -2, 1, 1.0, NAN);
    Test(0, 0, -2, NO_ROOTS, NAN, NAN);
    Test(0, 0, 0, INF_ROOTS, NAN, NAN);
    Test(0, 5, -10, 1, 2.0, NAN);
    Test(0, 5, 0, 1, 0, NAN);
    Test(0, 2000000000, 5000000000, 1, -2.5, NAN);
    Test(0, 0.001, 2000000000, 1, -2000000000000, NAN);
    Test(0, 9, -3, 1, 1.0/3.0 , NAN);

    TEST_PACK("\nQuadratic equat tests\n");
    Test(1, 0, -4, 2, 2, -2);
    Test(1, 0, 4, NO_ROOTS, NAN, NAN);
    Test(8, -4, 0, 2, 0.5, 0);
    Test(3, -27, 60, 2, 5, 4);
    Test(20, 4, 400, NO_ROOTS, NAN, NAN);
    Test(600000000, -5400000000, 12000000000, 2, 5, 4);
    Test(1, -6, 9, 2, 3, 3);
    
    return 0;
}

/* (c)Relayx (Nikita Zvezdin)
* Free for modification and distribution with the indication of the author
* GitHub: https://github.com/Relayx
*/