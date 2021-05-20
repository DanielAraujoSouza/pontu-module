#include "../include/calc.h"

int calc_factorial(int n)
{
	int result = 1;

	for (int c = 1; c <= n; c++)
		result *= c;

	return result;
}

int calc_binom_coeff(int n, int k)
{
	return calc_factorial(n) / (calc_factorial(k) * calc_factorial(n - k));
}

int calc_kronecker(int i, int j)
{
	return i == j ? 1 : 0;
}

real calc_squared_length3(real x, real y, real z)
{
	return ((x * x) + (y * y) + (z * z));
}

real calc_length3(real x, real y, real z)
{
	return sqrt((x * x) + (y * y) + (z * z));
}

real calc_squared_length2(real x, real y)
{
	return ((x * x) + (y * y));
}

real calc_length2(real x, real y)
{
	return sqrt((x * x) + (y * y));
}

real calc_randr(real max)
{
	return (real)rand() / (real)(RAND_MAX / max);
}

real calc_max2(real a, real b)
{
	return a > b ? a : b;
}

real calc_min2(real a, real b)
{
	return a < b ? a : b;
}

real calc_max3(real a, real b, real c)
{
	real t = a;

	if (b >= t)
		t = b;

	if (c >= t)
		t = c;

	return t;
}

real calc_sign(real a)
{
	return (a >= 0.0) ? 1.0 : -1.0;
}

real calc_logt(real a)
{
	return -1.0 * calc_sign(a) * log(fabs(a));
}

real calc_gaussian(real x, real s)
{
	real num = exp((-1.0 * x * x) / (2.0 * s * s));
	real den = s * sqrt(2.0 * CALC_PI);
	
	return num / den;
}

real calc_gaussian3(real x, real y, real z, real s)
{
	real num = exp(-1.0 * ((x * x) + (y * y) + (z * z)) / (2.0 * s * s));
	real den = pow(2.0 * CALC_PI * s * s, 1.5);
	
	return num / den;
}

