///------------------------------------------------------------------------------------------------------------
/// struct of function name, pointer to function, and differential function pointer
///------------------------------------------------------------------------------------------------------------

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "ASSert.h"
#include <math.h>

struct functions_t{
    char name[FUNCTIONMAXLEN] = {};
    double (*func)(double number);
    int value;
    int diff;
};
/// number of functions

double Abs(double number);
double ctan(double number);
double ctanh(double number);

functions_t functions[] = 
{
	{"cos", 	&cos, 	 0, -1},
	{"sin", 	&sin, 	 1,  0},
	{"abs", 	&Abs, 	 2, -1},
	{"tg", 		&tan, 	 3},
	{"ctg", 	&ctan, 	 4},
	{"sh", 		&sinh, 	 5,  6},
	{"ch", 		&cosh, 	 6,  5},
	{"th", 		&tanh, 	 7},
	{"cth", 	&ctanh,  8},
	{"ln", 		&log, 	 9, -1},
	{"log10", 	&log10, 10},
	{"exp", 	&exp, 	11, 11},
	{"acos", 	&acos, 	12},
	{"asin", 	&asin, 	13},
	{"atg", 	&atan, 	14},
	{"sqrt", 	&sqrt, 	15, -1}
};

double Abs(double number)
{
	return (number > 0)? number : -number;
}
double ctan(double number)
{
	assert(tan(number) != 0);
	return 1 / tan(number);
}
double ctanh(double number)
{
	return 1 / tanh(number);
}

#endif
