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
const double FuncNumber = 16;

double Abs(double number);
double ctan(double number);
double ctanh(double number);

functions_t functions[] = 
{
	{"cos", 	&cos, 	1000, 0},
	{"sin", 	&sin, 	1001, 1000},
	{"abs", 	&Abs, 	1002, 0},
	{"tg", 		&tan, 	1003},
	{"ctg", 	&ctan, 	1004},
	{"sh", 		&sinh, 	1005, 1006},
	{"ch", 		&cosh, 	1006, 1005},
	{"th", 		&tanh, 	1007},
	{"cth", 	&ctanh, 1008},
	{"ln", 		&log, 	1009, 0},
	{"log10", 	&log10, 1010},
	{"exp", 	&exp, 	1011, 1011},
	{"acos", 	&acos, 	1012},
	{"asin", 	&asin, 	1013},
	{"atg", 	&atan, 	1014},
	{"sqrt", 	&sqrt, 	1015, 0}
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