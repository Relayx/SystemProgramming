/*!
\file
\brief Header file with description of the functions

There are the main functions of the library
*/

#ifndef RELAYX_EQUATION_INCLUDED
#define RELAYX_EQUATION_INCLUDED

#include <assert.h>
#include <math.h>
#include <stdlib.h>

typedef enum 
{
    INF_ROOTS = -1,
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS
} Roots;

/*!
    Calculate simple linear equations!
    @param[in] k angular coefficient
    @param[in] l shift
    @param[out] x root of equation

    @return number of roots
    @version 1.0
    @authors Relayx
*/
Roots LinearEquation_ApiV1_0(double k, double b, double* x);

/*!
    Calculate quadratic equations!
    @param[in] a first coefficient
    @param[in] b second coefficient
    @param[in] c third coefficient
    @param[out] x1 first root of equation
    @param[out] x2 second root of equation

    @return number of roots
    @version 1.0
    @authors Relayx
*/
Roots QuadraticEquation_ApiV1_0(double a, double b, double c, 
                                double* x1, double* x2);

struct Equation_Api_v1_0
{
    Roots (*LinearEquation)(double k, double b, double* x);
    Roots (*QuadraticEquation)(double a, double b, double c, 
                               double* x1, double* x2);
};

typedef const struct Equation_Api_v1_0* Equation;

static Equation EquationInit()
{
    static const struct Equation_Api_v1_0 EquationApi = { LinearEquation_ApiV1_0,
                                                          QuadraticEquation_ApiV1_0};
    return &EquationApi;
}
                      
#endif

/* (c)Relayx (Nikita Zvezdin)
* Free for modification and distribution with the indication of the author
* GitHub: https://github.com/Relayx
*/