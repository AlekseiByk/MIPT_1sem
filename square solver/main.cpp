#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define MAXTRIES 100
#define MAXCOEFF 3
#define ZERO 1e-10
#define Inf_roots 3
#define test(a, b, c) if ((a) b (c))\
                          printf("");\
                      else\
                          printf("\neror");

/// \brief function for input erray of numbers with assertation.
/// \param prompt Invite string for user.
/// \return pointer to calloc() generated array, containig input values. Free after using.

double* InputNumbers(const char prompt[]);

/// \brief solves a line equation Ax + B = 0.
/// \param a x coefficient of equation.
/// \param b free coefficient of equation.
/// \param x pointer to the root.
/// \return returns number of roots. In case of any x, returns Inf_roots

int SolveLiner(double a, double b, double* x);

/// \breaf solves a quadratic equation Ax^2 + Bx + C = 0.
/// \param a x^2 coefficient of equation.
/// \param b x coefficient of equation.
/// \param c free coefficient of equation.
/// \param x1 pointer to the root.
/// \param x2 pointer to the root.
/// \return returns number of roots. In case of any x, returns Inf_roots

int solvesquare(double a, double b, double c, double* x1, double* x2);

/// \brief checking function for SquareSolver.

void unittest();

/// \brief compare double values
/// \param a first value
/// \param b second value
/// \param pr checking parametr
/// \return 1 if equal, 0 if not equal.

bool is_equal(double a,double b,double pr);

int main()
{

    printf("Squaresolver version 1.0\nBykov A.\n");
/*
    srand(4532);
    for (int i = 0; i < 10000; i++)
        unittest();
*/
    double* coeff = InputNumbers("Enter a, b, c:");
    double x1 = NAN, x2 = NAN;

    int nRoots=solvesquare( coeff[0], coeff[1], coeff[2], &x1, &x2);

    switch (nRoots)
    {
        case 0:
            printf("the equation has no roots\n");
            break;
        case 1:
            printf("the equation has one root: x = %lg\n", x1);
            break;
        case 2:
            printf("the equation has two roots: x1 = %lg, x2 = %lg\n", x1, x2);
            break;
        case Inf_roots:
            printf("the equation has infinity number of roots\n");
            break;
        default:
            printf("something wrong\n");
            break;
    }
    free(coeff);
    return 0;
}

double* InputNumbers(const char prompt[])
{
    double* coeff = (double *) calloc(MAXCOEFF, sizeof(coeff[0]));
    printf("%s\n", prompt);
    for (int i = 0; i < MAXTRIES; i++)
    {
        int sn = scanf("%lg %lg %lg", &coeff[0], &coeff[1], &coeff[2]);
        if ( sn == 3)
            return coeff;
        else
        {
            printf("Incorrect input\nPlease try again\n%s\n", prompt);
            fflush(stdin);
        }
    }
}

int SolveLiner(double a, double b, double* x)
{
    assert(isfinite(a));
    assert(isfinite(b));

    if (a == 0 && b == 0)
        return Inf_roots;
    else if (a == 0)
        return 0;
    else
    {
        *x = -b / a;
        return 1;
    }
}

int solvesquare(double a, double b, double c, double* x1, double* x2)
{
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    if (a == 0 && b == 0 && c == 0)
        return 3;
    else if (a== 0)
        return SolveLiner(b, c, x1);
    else if (b == 0)
    {
        *x1 = 0;
        return SolveLiner(a, b, x2) + 1;
    }
    else if (b == 0 && ((c > 0 && a > 0) || (c < 0 && a < 0)))
        return 0;
    else if (b == 0 && ((c > 0 && a < 0) || (c < 0 && a > 0)))
    {
        *x1 = sqrt(-c / a);
        *x2 = -sqrt(-c / a);
        return 2;
    }
    else
    {
        double d = b * b - 4 * a * c;
        if (d > 0)
        {
            //double q = sqrt;
            *x1 = (-b + sqrt(d)) / 2 / a;
            *x2 = (-b - sqrt(d)) / 2 / a;
            return 2;
        }
        else if (is_equal(d, 0, ZERO)) //1)branch predict
        {
            *x1 = -b / 2 / a;
            return 1;
        }
        else
            return 0;
    }
}

void unittest()
{
    double a = (rand()-16000) / 10000.;
    double b = (rand()-16000) / 10000.;
    double c = (rand()-16000) / 10000.;
    double x1 = NAN, x2 = NAN;
    int nRoots = solvesquare(a, b, c, &x1, &x2);

    switch (nRoots)
    {
        case 1:
            test(a * x1 * x1 + b * x1 + c, <=, ZERO)
            break;
        case 2:
            test(a * x1 * x1 + b * x1 + c, <=, ZERO)
            test(a * x2 * x2 + b * x2 + c, <=, ZERO)
            break;
        default:
            break;
    }
}

bool is_equal(double a, double b, double pr)
{
    double delta = (a - b > 0) ? (a - b) : (b - a);
    return (delta <= pr);
}



//#define test (a, b, c) if ((a) (b) (c)) ...

// test()