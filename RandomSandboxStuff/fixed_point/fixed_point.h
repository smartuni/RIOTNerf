#ifndef RIOTNERF_FIXED_POINT_H
#define RIOTNERF_FIXED_POINT_H
#include <stdint.h> /* int32_t, int64_t, uint32_t, uint64_t */

/*! a 32 bit signed fixed point number */
typedef struct
{
    int32_t v;
} fp32;

/*! a 64 bit signed fixed point number */
typedef struct 
{
    int64_t v;
} fp64;

/*! a 32 bit unsigned fixed point number */
typedef struct 
{
    uint32_t v;
} fpu32;

/*! a 64 bit unsigned fixed point number */
typedef struct 
{
    uint64_t v;
} fpu64;

/*! converts a double to an fp32 
 *  Note: the whole and fractional parts may not be too large to fit into a 32 bit integer.
**/
fp32 
double_to_fp32(double x);

/*! converts a double to an fp64 
 *  Note: the whole and fractional parts may not be too large to fit into a 64 bit integer.
**/
fp64
double_to_fp64(double x);

/*! converts a double to an fpu32 
 *  Note: the whole and fractional parts may not be too large to fit into a 32 bit integer.
 *  Note: do not use negative numbers.
**/
fpu32
double_to_fpu32(double x);

/*! converts a double to an fpu64 
 *  Note: the whole and fractional parts may not be too large to fit into a 64 bit integer.
 *  Note: do not use negative numbers.
**/
fpu64 
double_to_fpu64(double x);

/*! converts a float to an fp32 
 *  Note: the whole and fractional parts may not be too large to fit into a 32 bit integer.
**/
fp32
float_to_fp32(float x);

/*! converts a float to an fp64
 *  Note: the whole and fractional parts may not be too large to fit into a 64 bit integer.
**/
fp64
float_to_fp64(float x);

/*! converts a float to an fpu32 
 *  Note: the whole and fractional parts may not be too large to fit into a 32 bit integer.
 *  Note: do not use negative numbers.
**/
fpu32
float_to_fpu32(float x);

/*! converts a float to an fpu64
 *  Note: the whole and fractional parts may not be too large to fit into a 64 bit integer.
 *  Note: do not use negative numbers.
**/
fpu64
float_to_fpu64(float x);

/*! converts an fp32 to a double */
double
fp32_to_double(fp32 x);

/*! converts an fp64 to a double */
double
fp64_to_double(fp64 x);

/*! converts an fpu32 to a double */
double
fpu32_to_double(fpu32 x);

/*! converts an fpu64 to a double */
double
fpu64_to_double(fpu64 x);

/*! converts an fp32 to a float */
float
fp32_to_float(fp32 x);

/*! converts an fp64 to a float */
float
fp64_to_float(fp64 x);

/*! converts an fpu32 to a float */
float
fpu32_to_float(fpu32 x);

/*! converts an fpu64 to a float */
float
fpu64_to_float(fpu64 x);

/*! converts a 16 bit signed integer to an fp32 */
fp32
int_to_fp32(int16_t x);

/*! converts a 32 bit signed integer to an fp64 */
fp64
int_to_fp64(int32_t x);

/*! converts a 16 bit unsigned integer to an fpu32 */
fpu32
int_to_fpu32(uint16_t x);

/*! converts a 32 bit unsigned integer to an fpu64 */
fpu64
int_to_fpu64(uint32_t x);

/*! converts an fp32 to an integer;
 *  will occasionally return incorrect results for negative numbers or something. 
**/
int32_t
fp32_to_int(fp32 x);

/*! converts an fp64 to an integer;
 *  will occasionally return incorrect results for negative numbers or something. 
**/
int64_t
fp64_to_int(fp64 x);

/*! converts an fpu32 to an integer;
 *  will occasionally return incorrect results for negative numbers or something. 
**/
uint32_t
fpu32_to_int(fpu32 x);

/*! converts an fpu64 to an integer;
 *  will occasionally return incorrect results for negative numbers or something. 
**/
uint64_t
fpu64_to_int(fpu64 x);

/*! extracts the fractional part of an fp32. */
fp32
fp32_fraction(fp32 x);

/*! extracts the fractional part of an fp64. */
fp64
fp64_fraction(fp64 x);

/*! extracts the fractional part of an fpu32. */
fpu32
fpu32_fraction(fpu32 x);

/*! extracts the fractional part of an fpu64. */
fpu64
fpu64_fraction(fpu64 x);

/*! extracts the whole part of an fp32.
 *  Note: do not call this function with negative numbers.
**/
fp32
fp32_whole(fp32 x);

/*! extracts the whole part of an fp64.
 *  Note: do not call this function with negative numbers.
**/
fp64
fp64_whole(fp64 x);

/*! extracts the whole part of an fpu32.
 *  Note: do not call this function with negative numbers.
**/
fpu32
fpu32_whole(fpu32 x);

/*! extracts the whole part of an fpu64.
 *  Note: do not call this function with negative numbers.
**/
fpu64
fpu64_whole(fpu64 x);

/*! adds two fp32s.
 *  Note: be careful not to cause an overflow.
**/
fp32
fp32_add(fp32 a, fp32 b);

/*! adds two fp64s.
 *  Note: be careful not to cause an overflow.
**/
fp64
fp64_add(fp64 a, fp64 b);

/*! adds two fpu32s.
 *  Note: be careful not to cause an overflow.
**/
fpu32
fpu32_add(fpu32 a, fpu32 b);

/*! adds two fpu64s.
 *  Note: be careful not to cause an overflow.
**/
fpu64
fpu64_add(fpu64 a, fpu64 b);

/*! subtracts two fp32s. 
 *  Note: be careful not to cause an underflow.
**/
fp32
fp32_sub(fp32 a, fp32 b);

/*! subtracts two fp64s. 
 *  Note: be careful not to cause an underflow.
**/
fp64
fp64_sub(fp64 a, fp64 b);

/*! subtracts two fpu32s. 
 *  Note: be careful not to cause an underflow.
**/
fpu32
fpu32_sub(fpu32 a, fpu32 b);

/*! subtracts two fpu64s. 
 *  Note: be careful not to cause an underflow.
**/
fpu64
fpu64_sub(fpu64 a, fpu64 b);

/*! multiplies two fp32s.
 *  Note: be careful not to cause an overflow.
**/
fp32
fp32_mul(fp32 a, fp32 b);

/*! multiplies two fp64s.
 *  Note: be careful not to cause an overflow.
**/
fp64
fp64_mul(fp64 a, fp64 b);

/*! multiplies two fpu32s.
 *  Note: be careful not to cause an overflow.
**/
fpu32
fpu32_mul(fpu32 a, fpu32 b);

/*! multiplies two fpu64s.
 *  Note: be careful not to cause an overflow.
**/
fpu64
fpu64_mul(fpu64 a, fpu64 b);

/*! divides two fp32s.
 *  Note: a may not be larger than 127.
 *  Note: this operation may result in the loss of precision.
**/
fp32
fp32_div(fp32 a, fp32 b);

/*! divides two fp64s.
 *  Note: a may not be larger than 32767.
 *  Note: this operation may result in the loss of precision.
**/
fp64
fp64_div(fp64 a, fp64 b);

/*! divides two fpu32s.
 *  Note: a may not be larger than 255.
 *  Note: this operation may result in the loss of precision.
**/
fpu32
fpu32_div(fpu32 a, fpu32 b);

/*! divides two fpu64s.
 *  Note: a may not be larger than 65535.
 *  Note: this operation may result in the loss of precision.
**/
fpu64
fpu64_div(fpu64 a, fpu64 b);

#endif /* RIOTNERF_FIXED_POINT_H */
