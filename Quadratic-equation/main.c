#include <stdio.h>

#include "Equation/equation.h"

#define FLUSH "%*[^\n]%*c"

int main()
{
    Equation api;

    if((api = EquationInit()) == NULL)
    {
        assert(!"Fatal Error! Equation lib was not found");
        return 0;
    }

    printf("Quadratic equation solver\n");
    printf("(c)Relayx (Nikita Zvezdin) 2021 v1.0\n\n");

    printf("[a]x^2 + [b]x + [c] = 0\n");
    printf("Please, enter (a, b and c): ");

    double    a = NAN, b = NAN, c = NAN;
    const int ARGS_NUM = 3;

    while (scanf("%lg %lg %lg", &a, &b, &c) != ARGS_NUM)
    {
        printf("\nCoefficients are wrong. Please try again!\n");
        printf("[a]x^2 + [b]x + [c] = 0\n");
        printf("Please, enter (a, b and c): ");
        scanf(FLUSH); // Flushing standard input buffer
    }

    double x1 = NAN, x2= NAN;
    Roots  rootCount = api->QuadraticEquation(a, b, c, &x1, &x2);

    switch (rootCount)
    {
        case ONE_ROOT:
            printf("Root is: x = %lg\n", x1);
            break;

        case TWO_ROOTS:
            printf("Roots are: x1 = %lg, x2 = %lg\n", x1, x2);
            break;

        case NO_ROOTS:
            printf("There are no roots!\n");
            break; 

        case INF_ROOTS:
            printf("There are infinitely many roots!\n");
            break;

        default:
            printf("Something unexpected happened :(\n");
            break;
    }

    return 0;
}