#include "fixed_point.h"
#include <limits.h> /* CHAR_BIT */

/*! anonymous enum that contains constant integer expressions for the bit sizes of int32_t and int64_t */
enum
{
    bits32 = sizeof(int32_t) * CHAR_BIT, /* the number of bits of a 32bit integer */
    bits64 = sizeof(int64_t) * CHAR_BIT  /* the number of bits of a 64bit integer */
};

/*! enum that contains constant integer expressions for differenc scales */
enum scale
{
    scale32 = bits32 / 2, /* half of a 32bit integer */
    scale64 = bits64 / 2, /* half of a 64bit integer */

    mul_scale32 = scale32 / 2, /* a quarter of a 32bit integer */
    mul_scale64 = scale64 / 2, /* a quarter of a 64bit integer */ 

    div_scale32 = mul_scale32, /* a quarter of a 32bit integer */
    div_scale64 = mul_scale64  /* a quarter of a 64bit integer */
};

/*! fraction mask used to extract the fractional part of a 32bit value */
#define FM32    ((uint32_t) (UINT32_MAX >> (bits32 - scale32)))

/*! fraction mask used to extract the fractional part of a 64bit value */
#define FM64    ((uint64_t) (UINT64_MAX >> (bits64 - scale64)))

/*! mask used to extract the whole part of a 32bit value */
#define WM32    ((uint32_t) (~FM32))

/*! mask used to exttract the whole part of a 64bit value */
#define WM64    ((uint64_t) (~FM64))

/*! macro used to convert doubles to fixed point values */
#define FP_SCALED_DOUBLE_EXPR(signed_unsigned_one, scl) \
    (x * (double) ((signed_unsigned_one) << scl))

fp32 
double_to_fp32(double x) 
{
    fp32 ret;
    ret.v = FP_SCALED_DOUBLE_EXPR((int32_t) 1, scale32);
    return ret;
}

fp64 
double_to_fp64(double x) 
{
    fp64 ret;
    ret.v = FP_SCALED_DOUBLE_EXPR((int64_t) 1, scale64);
    return ret;
}

fpu32
double_to_fpu32(double x)
{
    fpu32 ret;
    ret.v = FP_SCALED_DOUBLE_EXPR((uint32_t) 1U, scale32);
    return ret;
}

fpu64
double_to_fpu64(double x)
{
    fpu64 ret;
    ret.v = FP_SCALED_DOUBLE_EXPR((uint64_t) 1U, scale64);
    return ret;
}

#undef FP_SCALED_DOUBLE_EXPR

/*! macro used to convert floats to fixed point values */
#define FP_SCALED_FLOAT_EXPR(signed_unsigned_one, scl) \
    (x * (float) ((signed_unsigned_one) << scl))

fp32
float_to_fp32(float x)
{
    fp32 ret;
    ret.v = FP_SCALED_FLOAT_EXPR((int32_t) 1, scale32);
    return ret;
}

fp64
float_to_fp64(float x)
{
    fp64 ret;
    ret.v = FP_SCALED_FLOAT_EXPR((int64_t) 1, scale64);
    return ret;
}

fpu32
float_to_fpu32(float x)
{
    fpu32 ret;
    ret.v = FP_SCALED_FLOAT_EXPR((uint32_t) 1U, scale32);
    return ret;
}

fpu64
float_to_fpu64(float x)
{
    fpu64 ret;
    ret.v = FP_SCALED_FLOAT_EXPR((uint64_t) 1U, scale64);
    return ret;
}

#undef FP_SCALED_FLOAT_EXPR

/*! macro used to convert fixed point values to doubles. */
#define FP_TO_DOUBLE(signed_unsigned, scl) \
    ((double) x.v / (double) ((signed_unsigned) << scl))

double
fp32_to_double(fp32 x)
{
    return FP_TO_DOUBLE((int32_t) 1, scale32);
}

double
fp64_to_double(fp64 x)
{
    return FP_TO_DOUBLE((int64_t) 1, scale64);
}

double
fpu32_to_double(fpu32 x)
{
    return FP_TO_DOUBLE((uint32_t) 1U, scale32);
}

double
fpu64_to_double(fpu64 x)
{
    return FP_TO_DOUBLE((uint64_t) 1U, scale64);
}

#undef FP_TO_DOUBLE

/*! macro used to convert fixed point values to floats. */
#define FP_TO_FLOAT(signed_unsigned, scl) \
    ((float) x.v / (float) ((signed_unsigned) << scl))

float
fp32_to_float(fp32 x)
{
    return FP_TO_FLOAT((int32_t) 1, scale32);
}

float
fp64_to_float(fp64 x)
{
    return FP_TO_FLOAT((int64_t) 1, scale64);
}

float
fpu32_to_float(fpu32 x)
{
    return FP_TO_FLOAT((uint32_t) 1U, scale32);
}

float
fpu64_to_float(fpu64 x)
{
    return FP_TO_FLOAT((uint64_t) 1U, scale64);
}

#undef FP_TO_FLOAT

/*! macro used to convert integers to fixed point values; 
 *  the integer may not be larger than the whole part of the fixed point type.
**/
#define INT_TO_FP(type, scl) (((type) x) << (scl))

fp32
int_to_fp32(int16_t x)
{
    fp32 ret;
    ret.v = INT_TO_FP(int32_t, scale32);
    return ret;
}

fp64
int_to_fp64(int32_t x)
{
    fp64 ret;
    ret.v = INT_TO_FP(int64_t ,scale64);
    return ret;
}

fpu32
int_to_fpu32(uint16_t x)
{
    fpu32 ret;
    ret.v = INT_TO_FP(uint32_t ,scale32);
    return ret;
}

fpu64
int_to_fpu64(uint32_t x)
{
    fpu64 ret;
    ret.v = INT_TO_FP(uint64_t ,scale64);
    return ret;
}

#undef INT_TO_FP

/*! macro used to convert fixed point values to integers by dropping the fractional part 
 *  Note: will not work with negative numbers. 
**/
#define FP_TO_INT(scl)  (x.v >> (scl))

int32_t
fp32_to_int(fp32 x)
{
    return FP_TO_INT(scale32);
}

int64_t
fp64_to_int(fp64 x)
{
    return FP_TO_INT(scale64);
}

uint32_t
fpu32_to_int(fpu32 x)
{
    return FP_TO_INT(scale32);
}

uint64_t
fpu64_to_int(fpu64 x)
{
    return FP_TO_INT(scale64);
}

#undef FP_TO_INT

/*! macro used to apply bit masks to fixed point values */
#define FP_APPLY_MASK(mask) \
    (x.v & (mask))

fp32
fp32_fraction(fp32 x)
{
    fp32 ret;
    ret.v = FP_APPLY_MASK(FM32);
    return ret;
}

fp64
fp64_fraction(fp64 x)
{
    fp64 ret;
    ret.v = FP_APPLY_MASK(FM64);
    return ret;
}

fpu32
fpu32_fraction(fpu32 x)
{
    fpu32 ret;
    ret.v = FP_APPLY_MASK(FM32);
    return ret;
}

fpu64
fpu64_fraction(fpu64 x)
{
    fpu64 ret;
    ret.v = FP_APPLY_MASK(FM64);
    return ret;
}

fp32
fp32_whole(fp32 x)
{
    fp32 ret;
    ret.v = FP_APPLY_MASK(WM32);
    return ret;
}

fp64
fp64_whole(fp64 x)
{
    fp64 ret;
    ret.v = FP_APPLY_MASK(WM64);
    return ret;
}

fpu32
fpu32_whole(fpu32 x)
{
    fpu32 ret;
    ret.v = FP_APPLY_MASK(WM32);
    return ret;
}

fpu64
fpu64_whole(fpu64 x)
{
    fpu64 ret;
    ret.v = FP_APPLY_MASK(WM64);
    return ret;
}

#undef FP_APPLY_MASK

/*! macro used to add two fixed point values using integer addition */
#define FP_ADD \
    (a.v + b.v)

fp32
fp32_add(fp32 a, fp32 b)
{
    fp32 ret;
    ret.v = FP_ADD;
    return ret;
}

fp64
fp64_add(fp64 a, fp64 b)
{
    fp64 ret;
    ret.v = FP_ADD;
    return ret;
}

fpu32
fpu32_add(fpu32 a, fpu32 b)
{
    fpu32 ret;
    ret.v = FP_ADD;
    return ret;
}

fpu64
fpu64_add(fpu64 a, fpu64 b)
{
    fpu64 ret;
    ret.v = FP_ADD;
    return ret;
}

#undef FP_ADD

/*! macro used to subtract two fixed point values using integer subtraction */
#define FP_SUB \
    (a.v - b.v)

fp32
fp32_sub(fp32 a, fp32 b)
{
    fp32 ret;
    ret.v = FP_SUB;
    return ret;
}

fp64
fp64_sub(fp64 a, fp64 b)
{
    fp64 ret;
    ret.v = FP_SUB;
    return ret;
}

fpu32
fpu32_sub(fpu32 a, fpu32 b)
{
    fpu32 ret;
    ret.v = FP_SUB;
    return ret;
}

fpu64
fpu64_sub(fpu64 a, fpu64 b)
{
    fpu64 ret;
    ret.v = FP_SUB;
    return ret;
}

#undef FP_SUB

/*! macro used to multiply two fixed point values */
#define FP_MUL(scale, mul_scale1, mul_scale2) \
    (((a.v >> (mul_scale1)) * \
    (b.v >> (mul_scale2))) \
    >> ((scale) - ((mul_scale1) + (mul_scale2))))

fp32
fp32_mul(fp32 a, fp32 b)
{
    fp32 ret;
    ret.v = FP_MUL(scale32, mul_scale32, mul_scale32);
    return ret;
}

fp64
fp64_mul(fp64 a, fp64 b)
{
    fp64 ret;
    ret.v = FP_MUL(scale64, mul_scale64, mul_scale64);
    return ret;
}

fpu32
fpu32_mul(fpu32 a, fpu32 b)
{
    fpu32 ret;
    ret.v = FP_MUL(scale32, mul_scale32, mul_scale32);
    return ret;
}

fpu64
fpu64_mul(fpu64 a, fpu64 b)
{
    fpu64 ret;
    ret.v = FP_MUL(scale64, mul_scale64, mul_scale64);
    return ret;
}

#undef FP_MUL

/*! macro used to divide two fixed point values */
#define FP_DIV(div_scale1, div_scale2) \
    (((a.v) << (div_scale1)) / \
    (b.v) << (div_scale2))

fp32
fp32_div(fp32 a, fp32 b)
{
    fp32 ret;
    ret.v = FP_DIV(div_scale32, div_scale32);
    return ret;
}

fp64
fp64_div(fp64 a, fp64 b)
{
    fp64 ret;
    ret.v = FP_DIV(div_scale64, div_scale64);
    return ret;
}

fpu32
fpu32_div(fpu32 a, fpu32 b)
{
    fpu32 ret;
    ret.v = FP_DIV(div_scale32, div_scale32);
    return ret;
}

fpu64
fpu64_div(fpu64 a, fpu64 b)
{
    fpu64 ret;
    ret.v = FP_DIV(div_scale64, div_scale64 );
    return ret;
}
