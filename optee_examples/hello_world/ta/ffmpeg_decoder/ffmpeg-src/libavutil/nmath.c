#include "nmath.h"
// #include "../../../math.h"
#include "nfrexp.h"

# define HUGE_VAL 1e10000

# define M_E		2.7182818284590452354	/* e */
# define M_LOG2E	1.4426950408889634074	/* log_2 e */
# define M_LOG10E	0.43429448190325182765	/* log_10 e */
# define M_LN2		0.69314718055994530942	/* log_e 2 */
# define M_LN10		2.30258509299404568402	/* log_e 10 */
# define M_PI		3.14159265358979323846	/* pi */
# define M_PI_2		1.57079632679489661923	/* pi/2 */
# define M_PI_4		0.78539816339744830962	/* pi/4 */
# define M_1_PI		0.31830988618379067154	/* 1/pi */
# define M_2_PI		0.63661977236758134308	/* 2/pi */
# define M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
# define M_SQRT2	1.41421356237309504880	/* sqrt(2) */
# define M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */
# define M_PI_M_2 2*M_PI
# define PI M_PI

/* macros for constructing polynomials */
#define	POLYNOM1(x, a)	((a)[1]*(x)+(a)[0])
#define	POLYNOM2(x, a)	(POLYNOM1((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM3(x, a)	(POLYNOM2((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM4(x, a)	(POLYNOM3((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM5(x, a)	(POLYNOM4((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM6(x, a)	(POLYNOM5((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM7(x, a)	(POLYNOM6((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM8(x, a)	(POLYNOM7((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM9(x, a)	(POLYNOM8((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM10(x, a)	(POLYNOM9((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM11(x, a)	(POLYNOM10((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM12(x, a)	(POLYNOM11((x),(a)+1)*(x)+(a)[0])
#define	POLYNOM13(x, a)	(POLYNOM12((x),(a)+1)*(x)+(a)[0])

/* Largest signed long int power of 2 */
#define MAXSHIFT        (8 * sizeof(long) - 2)   

#define	MAXFLOAT	1.7e308

double ldexp(value, exp)
     double value;
     int    exp; 
{
	extern double frexp();
	int	old_exp;

        if (exp == 0 || value == 0.0) /* nothing to do for zero */
                return (value);
        (void) frexp(value, &old_exp);
        if (exp > 0) {
                if (exp + old_exp > 1023) { /* overflow */
                        return (value < 0 ? -MAXFLOAT : MAXFLOAT);
                }
                for ( ; exp > MAXSHIFT; exp -= MAXSHIFT)
                        value *= (1L << MAXSHIFT);
                return (value * (1L << exp));
        }
        if (exp + old_exp < -1023) { /* underflow */
                return (0.0);
        }
        for ( ; exp < -MAXSHIFT; exp += MAXSHIFT)
                value *= 1.0/(1L << MAXSHIFT); /* mult faster than div */
        return (value / (1L << -exp));
}

int isfinite (double x) {
    return (x >= HUGE_VAL);
}

double hypot (double x, double y)
{
  if (isfinite (x) && isfinite (y))
    {
      /* Determine absolute values.  */
      x = fabs (x);
      y = fabs (y);

      {
        /* Find the bigger and the smaller one.  */
        double a;
        double b;

        if (x >= y)
          {
            a = x;
            b = y;
          }
        else
          {
            a = y;
            b = x;
          }
        /* Now 0 <= b <= a.  */

        {
          int e;
          double an;
          double bn;

          /* Write a = an * 2^e, b = bn * 2^e with 0 <= bn <= an < 1.  */
          an = frexp (a, &e);
          bn = ldexp (b, - e);

          {
            double cn;

            /* Through the normalization, no unneeded overflow or underflow
               will occur here.  */
            cn = sqrt (an * an + bn * bn);
            return ldexp (cn, e);
          }
        }
      }
    }
  else
    {
      if (isinf (x) || isinf (y))
        /* x or y is infinite.  Return +Infinity.  */
        return HUGE_VAL;
      else
        /* x or y is NaN.  Return NaN.  */
        return x + y;
    }
}

int __IsNan(double d)
{
#if defined(vax) || defined(pdp)
#else
	float f = d;

	if ((*((long *) &f) & 0x7f800000) == 0x7f800000 &&
	    (*((long *) &f) & 0x007fffff) != 0) return 1;
#endif
	return 0;
}

static double
asin_acos(double x, int cosfl)
{
	int negative = x < 0;
	int     i;
	double  g;
	static double p[] = {
		-0.27368494524164255994e+2,
		 0.57208227877891731407e+2,
		-0.39688862997540877339e+2,
		 0.10152522233806463645e+2,
		-0.69674573447350646411e+0
	};
	static double q[] = {
		-0.16421096714498560795e+3,
		 0.41714430248260412556e+3,
		-0.38186303361750149284e+3,
		 0.15095270841030604719e+3,
		-0.23823859153670238830e+2,
		 1.0
	};

	if (__IsNan(x)) {
		return x;
	}

	if (negative) {
		x = -x;
	}
	if (x > 0.5) {
		i = 1;
		if (x > 1) {
			return 0;
		}
		g = 0.5 - 0.5 * x;
		x = - sqrt(g);
		x += x;
	}
	else {
		/* ??? avoid underflow ??? */
		i = 0;
		g = x * x;
	}
	x += x * g * POLYNOM4(g, p) / POLYNOM5(g, q);
	if (cosfl) {
		if (! negative) x = -x;
	}
	if ((cosfl == 0) == (i == 1)) {
		x = (x + M_PI_4) + M_PI_4;
	}
	else if (cosfl && negative && i == 1) {
		x = (x + M_PI_2) + M_PI_2;
	}
	if (! cosfl && negative) x = -x;
	return x;
}

double
asin(double x)
{
	return asin_acos(x, 0);
}

double
acos(double x)
{
	return asin_acos(x, 1);
}

double atan(double x) {
	return asin(x / sqrt(x * x + 1));
}

double atan2(double y, double x){
  double a;
  a = 0.0;

  if(x > 0.0){
    a = atan(y/x);
  }else if(x < 0.0 && y >= 0.0){
    a = atan(y/x) + M_PI;
  }else if(x < 0.0 && y < 0.0){
    a = atan(y/x) - M_PI;
  }else if(x == 0.0 && y > 0.0){
    a = M_PI/2.0;
  }else if(x == 0.0 && y < 0.0){
    a =  -M_PI/2.0;
  }

  return a;
}

int compare_float(double f1, double f2)
{
 double precision = 0.00000000000000000001;
 if ((f1 - precision) < f2)
  {
 return -1;
  }
 else if ((f1 + precision) > f2)
 {
  return 1;
 }
 else
  {
 return 0;
  }
}

double cos(double x){
 if( x < 0.0f ) 
  x = -x;

  if (0 <= compare_float(x,M_PI_M_2)) 
 {
 do {
  x -= M_PI_M_2;
  }while(0 <= compare_float(x,M_PI_M_2));

  }

  if ((0 <= compare_float(x, M_PI)) && (-1 == compare_float(x, M_PI_M_2)))
  {
   x -= M_PI;
   return ((-1)*(1.0f - (x*x/2.0f)*( 1.0f - (x*x/12.0f) * ( 1.0f - (x*x/30.0f) * (1.0f - (x*x/56.0f )*(1.0f - (x*x/90.0f)*(1.0f - (x*x/132.0f)*(1.0f - (x*x/182.0f)))))))));
  } 
 return 1.0f - (x*x/2.0f)*( 1.0f - (x*x/12.0f) * ( 1.0f - (x*x/30.0f) * (1.0f - (x*x/56.0f )*(1.0f - (x*x/90.0f)*(1.0f - (x*x/132.0f)*(1.0f - (x*x/182.0f)))))));
}

#define EPSILON .0000000000001
// this is smallest effective threshold, at least on my OS (WSL ubuntu 18)
// possibly because factorial part turns 0 at some point
// and it happens faster then series element turns 0;
// validation was made against sin() from <math.h>
double sin(double x)
{
    int k = 2;
    double r = x;
    double acc = 1;
    double den = 1;
    double num = x;

//  precision drops rapidly when x is not close to 0
//  so move x to 0 as close as possible
    while (x > PI)
        x -= PI;
    while (x < -PI)
        x += PI;
    if (x > PI / 2)
        return (sin(PI - x));
    if (x < -PI / 2)
        return (sin(-PI - x));
//  not using fabs for performance reasons
    while (acc > EPSILON || acc < -EPSILON)
    {
        num *= -x * x;
        den *= k * (k + 1);
        acc = num / den;
        r += acc;
        k += 2;
    }
    return (r);
}

double exp(double x) 
{ 
    const double epsilon = 1e-7; 
    double sum = 0.0; 
    int n = 0; 
    double factorial = 1; 
    double power=1.0; 
    double term; 
    do { 
        term = power/factorial; 
        sum += term; 
        n += 1; 
        power *= x; 
        factorial *=n; 
    } while (fabs(term)>=epsilon); 
    return sum; 
} 

// int pow(int base, int exp)
// {
//     if(exp < 0)
//     return -1;

//     int result = 1;
//     while (exp)
//     {
//         if (exp & 1)
//             result *= base;
//         exp >>= 1;
//         base *= base;
//     }

//     return result;
// }

float pow(float x, int y)
{
    float temp;
    if( y == 0)
       return 1;
    temp = pow(x, y/2);       
    if (y%2 == 0)
        return temp*temp;
    else
    {
        if(y > 0)
            return x*temp*temp;
        else
            return (temp*temp)/x;
    }
}

#define FM_DOUBLE_BIAS 1023
#define FM_DOUBLE_EMASK 2146435072
#define FM_DOUBLE_MBITS 20
#define FM_DOUBLE_MMASK 1048575
#define FM_DOUBLE_EZERO 1072693248

/* generate 2**num in floating point by bitshifting */
#define FM_DOUBLE_INIT_EXP(var,num)                 \
    var.s.i0 = 0;                                     \
    var.s.i1 = (((int) num) + FM_DOUBLE_BIAS) << 20

typedef unsigned long long uint64_t;
typedef long int32_t;

typedef union 
{
    double   f;
    uint64_t u;
    struct {int32_t  i0,i1;} s;
}  udi_t;

static const double fm_exp2_q[] __attribute__ ((aligned(16))) = {
/*  1.00000000000000000000e0, */
    2.33184211722314911771e2,
    4.36821166879210612817e3
};
static const double fm_exp2_p[] __attribute__ ((aligned(16))) = {
    2.30933477057345225087e-2,
    2.02020656693165307700e1,
    1.51390680115615096133e3
};

double fm_exp2(double x)
{
    double   ipart, fpart, px, qx;
    udi_t    epart;

    ipart = __builtin_floor(x+0.5);
    fpart = x - ipart;
    FM_DOUBLE_INIT_EXP(epart,ipart);

    x = fpart*fpart;

    px =        fm_exp2_p[0];
    px = px*x + fm_exp2_p[1];
    qx =    x + fm_exp2_q[0];
    px = px*x + fm_exp2_p[2];
    qx = qx*x + fm_exp2_q[1];

    px = px * fpart;

    x = 1.0 + 2.0*(px/(qx-px));
    return epart.f*x;
}