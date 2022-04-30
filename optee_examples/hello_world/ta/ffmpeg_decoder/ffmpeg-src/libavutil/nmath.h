#ifndef NMATH
#define NMATH

double hypot (double x, double y);
double atan2(double y, double x);
double cos(double x);
double sin(double x);
double exp(double x);
// int pow(int base, int exp);
float pow(float x, int y);
double fm_exp2(double x);
#define exp2(x) fm_exp2(x)

#endif // for NMATH