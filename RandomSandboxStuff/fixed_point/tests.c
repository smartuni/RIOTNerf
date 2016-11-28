#include "tests.h"
#include "fixed_point.h" /* fp32, fp64, fpu32, fpu64, ... */
#include <stdio.h> /* vfprintf */
#include <stdarg.h> /* va_list, va_start, va_end */
#include <stddef.h> /* size_t */
#include <math.h> /* M_PI */

/*! a float representation of pi */
#define FLOAT_PI ((float) M_PI)

/*! the count of elements of an array.
 *  Note: beware of array to pointer decay;
 *  Note: will give an incorrect count if used on a pointer to the beginning of an array.
 *  Note: only use this macro on actual arrays
**/
#define COUNT_OF(ary)   (sizeof((ary)) / sizeof(*(ary)))

/*! counts how many tests have been run */
int tests_run = 0;

/*! the type of a pointer to a test function;
 * used to store pointers to the test functions in the test_functions array 
**/
typedef struct test_result (*test_function)(void);

/*! a printf-style function that prints to stderr, rather than stdout */
static int
printf_stderr(char const *fmt, ...)
{
    va_list arg;
    int done;

    va_start(arg, fmt);
    done = vfprintf(stderr, fmt, arg);
    va_end(arg);

    return done;
}

/*! macro used to test if v is within lower_bound and upper_bound
 *  lower_bound must be <= upper_bound
**/
#define TEST_IS_WITHIN(v, lower_bound, upper_bound) \
    (((v) >= (lower_bound)) && ((v) <= (upper_bound)))

/*! compares doubles for (almost) equality */
static bool
double_almost_equal(double a, double b)
{
    static double const delta = 0.02;
    return TEST_IS_WITHIN(a, b - delta, b + delta);
}

/*! compares floats for (almost) equality */
static bool
float_almost_equal(float a, float b)
{
    static float const delta = 0.02F;
    return TEST_IS_WITHIN(a, b - delta, b + delta);
}

/*! test function to test double conversions with fp32 */
TEST_CASE(fp32_double_conversions)
    fp32 zero = double_to_fp32(0.0);
    TEST_ASSERT(double_almost_equal(fp32_to_double(zero), 0.0));

    fp32 negative_one = double_to_fp32(-1.0);
    TEST_ASSERT(double_almost_equal(fp32_to_double(negative_one), -1.0));

    fp32 pi = double_to_fp32(M_PI);
    TEST_ASSERT(double_almost_equal(fp32_to_double(pi), M_PI));
TEST_CASE_END

/*! test function to test double conversions with fp64 */
TEST_CASE(fp64_double_conversions)
    fp64 zero = double_to_fp64(0.0);
    TEST_ASSERT(double_almost_equal(fp64_to_double(zero), 0.0));

    fp64 negative_one = double_to_fp64(-1.0);
    TEST_ASSERT(double_almost_equal(fp64_to_double(negative_one), -1.0));

    fp64 pi = double_to_fp64(M_PI);
    TEST_ASSERT(double_almost_equal(fp64_to_double(pi), M_PI));
TEST_CASE_END

/*! test function to test double conversions with fpu32 */
TEST_CASE(fpu32_double_conversions)
    fpu32 zero = double_to_fpu32(0.0);
    TEST_ASSERT(double_almost_equal(fpu32_to_double(zero), 0.0));

    fpu32 one = double_to_fpu32(1.0);
    TEST_ASSERT(double_almost_equal(fpu32_to_double(one), 1.0));

    fpu32 pi = double_to_fpu32(M_PI);
    TEST_ASSERT(double_almost_equal(fpu32_to_double(pi), M_PI));
TEST_CASE_END

/*! test function to test double conversions with fpu64 */
TEST_CASE(fpu64_double_conversions)
    fpu64 zero = double_to_fpu64(0.0);
    TEST_ASSERT(double_almost_equal(fpu64_to_double(zero), 0.0));

    fpu64 one = double_to_fpu64(1.0);
    TEST_ASSERT(double_almost_equal(fpu64_to_double(one), 1.0));

    fpu64 pi = double_to_fpu64(M_PI);
    TEST_ASSERT(double_almost_equal(fpu64_to_double(pi), M_PI));
TEST_CASE_END

/*! test function to test float conversions with fp32 */
TEST_CASE(fp32_float_conversions)
    fp32 zero = float_to_fp32(0.0F);
    TEST_ASSERT(float_almost_equal(fp32_to_float(zero), 0.0F));

    fp32 negative_one = float_to_fp32(-1.0F);
    TEST_ASSERT(float_almost_equal(fp32_to_float(negative_one), -1.0F));

    fp32 pi = float_to_fp32(FLOAT_PI);
    TEST_ASSERT(float_almost_equal(fp32_to_float(pi), FLOAT_PI));
TEST_CASE_END

/*! test function to test float conversions with fp64 */
TEST_CASE(fp64_float_conversions)
    fp64 zero = float_to_fp64(0.0F);
    TEST_ASSERT(float_almost_equal(fp64_to_float(zero), 0.0F));

    fp64 negative_one = float_to_fp64(-1.0F);
    TEST_ASSERT(float_almost_equal(fp64_to_float(negative_one), -1.0F));

    fp64 pi = float_to_fp64(FLOAT_PI);
    TEST_ASSERT(float_almost_equal(fp64_to_float(pi), FLOAT_PI));
TEST_CASE_END

/*! test function to test float conversions with fpu32 */
TEST_CASE(fpu32_float_conversions)
    fpu32 zero = float_to_fpu32(0.0F);
    TEST_ASSERT(float_almost_equal(fpu32_to_float(zero), 0.0F));

    fpu32 one = float_to_fpu32(1.0F);
    TEST_ASSERT(float_almost_equal(fpu32_to_float(one), 1.0F));

    fpu32 pi = float_to_fpu32(FLOAT_PI);
    TEST_ASSERT(float_almost_equal(fpu32_to_float(pi), FLOAT_PI));
TEST_CASE_END

/*! test function to test float conversions with fpu64 */
TEST_CASE(fpu64_float_conversions)
    fpu64 zero = float_to_fpu64(0.0F);
    TEST_ASSERT(float_almost_equal(fpu64_to_float(zero), 0.0F));

    fpu64 one = float_to_fpu64(1.0F);
    TEST_ASSERT(float_almost_equal(fpu64_to_float(one), 1.0F));

    fpu64 pi = float_to_fpu64(FLOAT_PI);
    TEST_ASSERT(float_almost_equal(fpu64_to_float(pi), FLOAT_PI));
TEST_CASE_END

/*! test function to test int conversions with fp32 */
TEST_CASE(fp32_int_conversions)
    fp32 zero = int_to_fp32(0);
    TEST_ASSERT(fp32_to_int(zero) == 0);

    fp32 negative_one = int_to_fp32(-1);
    TEST_ASSERT(fp32_to_int(negative_one) == -1);

    fp32 min = int_to_fp32(INT16_MIN);
    TEST_ASSERT(fp32_to_int(min) == INT16_MIN);

    fp32 max = int_to_fp32(INT16_MAX);
    TEST_ASSERT(fp32_to_int(max) == INT16_MAX);

    fp32 from_double = double_to_fp32(123.4567890);
    TEST_ASSERT(fp32_to_int(from_double) == 123);
TEST_CASE_END

/*! test function to test int conversions with fp64 */
TEST_CASE(fp64_int_conversions)
    fp64 zero = int_to_fp64(0);
    TEST_ASSERT(fp64_to_int(zero) == 0);

    fp64 negative_one = int_to_fp64(-1);
    TEST_ASSERT(fp64_to_int(negative_one) == -1);

    fp64 min = int_to_fp64(INT32_MIN);
    TEST_ASSERT(fp64_to_int(min) == INT32_MIN);

    fp64 max = int_to_fp64(INT32_MAX);
    TEST_ASSERT(fp64_to_int(max) == INT32_MAX);

    fp64 from_double = double_to_fp64(123.4567890);
    TEST_ASSERT(fp64_to_int(from_double) == 123);
TEST_CASE_END

/*! test function to test int conversions with fpu32 */
TEST_CASE(fpu32_int_conversions)
    fpu32 zero = int_to_fpu32(0);
    TEST_ASSERT(fpu32_to_int(zero) == 0);

    fpu32 one = int_to_fpu32(1);
    TEST_ASSERT(fpu32_to_int(one) == 1);

    fpu32 max = int_to_fpu32(UINT16_MAX);
    TEST_ASSERT(fpu32_to_int(max) == UINT16_MAX);

    fpu32 from_double = double_to_fpu32(123.4567890);
    TEST_ASSERT(fpu32_to_int(from_double) == 123);
TEST_CASE_END

/*! test function to test int conversions with fpu64 */
TEST_CASE(fpu64_int_conversions)
    fpu64 zero = int_to_fpu64(0);
    TEST_ASSERT(fpu64_to_int(zero) == 0);

    fpu64 one = int_to_fpu64(1);
    TEST_ASSERT(fpu64_to_int(one) == 1);

    fpu64 max = int_to_fpu64(UINT32_MAX);
    TEST_ASSERT(fpu64_to_int(max) == UINT32_MAX);

    fpu64 from_double = double_to_fpu64(123.4567890);
    TEST_ASSERT(fpu64_to_int(from_double) == 123);
TEST_CASE_END

/*! test function used to test the extraction of fractional parts from fixed point numbers */
TEST_CASE(fractions)
    fp32 v32 = double_to_fp32(123.789456123);
    fp32 f32 = fp32_fraction(v32);
    TEST_ASSERT(double_almost_equal(fp32_to_double(f32), 0.789456123));

    fp64 v64 = double_to_fp64(123.789456123);
    fp64 f64 = fp64_fraction(v64);
    TEST_ASSERT(double_almost_equal(fp64_to_double(f64), 0.789456123));

    fpu32 vu32 = double_to_fpu32(123.789456123);
    fpu32 fu32 = fpu32_fraction(vu32);
    TEST_ASSERT(double_almost_equal(fpu32_to_double(fu32), 0.789456123));

    fpu64 vu64 = double_to_fpu64(123.789456123);
    fpu64 fu64 = fpu64_fraction(vu64);
    TEST_ASSERT(double_almost_equal(fpu64_to_double(fu64), 0.789456123));
TEST_CASE_END

/*! test function used to test the extraction of whole parts from fixed point numbers */
TEST_CASE(whole_parts)
    fp32 v32 = double_to_fp32(12378.456123);
    fp32 w32 = fp32_whole(v32);
    TEST_ASSERT(double_almost_equal(fp32_to_double(w32), 12378.0));

    fp64 v64 = double_to_fp64(123789999.456123);
    fp64 w64 = fp64_whole(v64);
    TEST_ASSERT(double_almost_equal(fp64_to_double(w64), 123789999.0));

    fpu32 vu32 = double_to_fpu32(12378.456123);
    fpu32 wu32 = fpu32_whole(vu32);
    TEST_ASSERT(double_almost_equal(fpu32_to_double(wu32), 12378.0));

    fpu64 vu64 = double_to_fpu64(123789999.456123);
    fpu64 wu64 = fpu64_whole(vu64);
    TEST_ASSERT(double_almost_equal(fpu64_to_double(wu64), 123789999.0));
TEST_CASE_END

/*! test function used to test addition of fixed point numbers */
TEST_CASE(add)
    fp32 v1_32 = double_to_fp32(50.50);
    fp32 v2_32 = double_to_fp32(70.70);
    fp32 r32 = fp32_add(v1_32, v2_32);
    TEST_ASSERT(double_almost_equal(fp32_to_double(r32), 121.2));

    fp32 v1_32_a = double_to_fp32(-10.1);
    fp32 v2_32_a = double_to_fp32(12.3);
    fp32 r32_a = fp32_add(v1_32_a, v2_32_a);
    TEST_ASSERT(double_almost_equal(fp32_to_double(r32_a), 2.2));

    fp64 v1_64 = double_to_fp64(50.50);
    fp64 v2_64 = double_to_fp64(70.70);
    fp64 r64 = fp64_add(v1_64, v2_64);
    TEST_ASSERT(double_almost_equal(fp64_to_double(r64), 121.2));

    fp64 v1_64_a = double_to_fp64(-10.1);
    fp64 v2_64_a = double_to_fp64(12.3);
    fp64 r64_a = fp64_add(v1_64_a, v2_64_a);
    TEST_ASSERT(double_almost_equal(fp64_to_double(r64_a), 2.2));

    fpu32 v1_u32 = double_to_fpu32(50.50);
    fpu32 v2_u32 = double_to_fpu32(70.70);
    fpu32 ru32 = fpu32_add(v1_u32, v2_u32);
    TEST_ASSERT(double_almost_equal(fpu32_to_double(ru32), 121.2));

    fpu64 v1_u64 = double_to_fpu64(50.50);
    fpu64 v2_u64 = double_to_fpu64(70.70);
    fpu64 ru64 = fpu64_add(v1_u64, v2_u64);
    TEST_ASSERT(double_almost_equal(fpu64_to_double(ru64), 121.2));
TEST_CASE_END

/*! test function used to test subtraction of fixed point numbers */
TEST_CASE(sub)
    fp32 v1_32 = double_to_fp32(10.01);
    fp32 v2_32 = double_to_fp32(11.5);
    fp32 r32 = fp32_sub(v1_32, v2_32);
    TEST_ASSERT(double_almost_equal(fp32_to_double(r32), -1.49));

    fp64 v1_64 = double_to_fp64(10.01);
    fp64 v2_64 = double_to_fp64(11.5);
    fp64 r64 = fp64_sub(v1_64, v2_64);
    TEST_ASSERT(double_almost_equal(fp64_to_double(r64), -1.49));

    fpu32 v1_u32 = double_to_fpu32(5.0);
    fpu32 v2_u32 = double_to_fpu32(3.51294);
    fpu32 ru32 = fpu32_sub(v1_u32, v2_u32);
    TEST_ASSERT(double_almost_equal(fpu32_to_double(ru32), 1.48706));

    fpu64 v1_u64 = double_to_fpu64(5.0);
    fpu64 v2_u64 = double_to_fpu64(3.51294);
    fpu64 ru64 = fpu64_sub(v1_u64, v2_u64);
    TEST_ASSERT(double_almost_equal(fpu64_to_double(ru64), 1.48706));
TEST_CASE_END

/*! test function used to test multiplication of fixed point numbers */
TEST_CASE(mul)
    fp32 v1_32 = int_to_fp32(3);
    fp32 v2_32 = float_to_fp32(7.41F);
    fp32 r32 = fp32_mul(v1_32, v2_32);
    TEST_ASSERT(double_almost_equal(fp32_to_double(r32), 22.23));

    fp64 v1_64 = int_to_fp64(3);
    fp64 v2_64 = float_to_fp64(7.41F);
    fp64 r64 = fp64_mul(v1_64, v2_64);
    TEST_ASSERT(double_almost_equal(fp64_to_double(r64), 22.23));

    fpu32 v1_u32 = int_to_fpu32(3);
    fpu32 v2_u32 = float_to_fpu32(7.41F);
    fpu32 ru32 = fpu32_mul(v1_u32, v2_u32);
    TEST_ASSERT(double_almost_equal(fpu32_to_double(ru32), 22.23));

    fpu64 v1_u64 = int_to_fpu64(3);
    fpu64 v2_u64 = float_to_fpu64(7.41F);
    fpu64 ru64 = fpu64_mul(v1_u64, v2_u64);
    TEST_ASSERT(double_almost_equal(fpu64_to_double(ru64), 22.23));
TEST_CASE_END

/*! test function used to test division of fixed point numbers */
TEST_CASE(division)
    fp32 v1_32 = double_to_fp32(5.0);
    fp32 v2_32 = double_to_fp32(2.0);
    fp32 r32 = fp32_div(v1_32, v2_32);
    TEST_ASSERT(double_almost_equal(fp32_to_double(r32), 2.5));

    fp64 v1_64 = double_to_fp64(5.0);
    fp64 v2_64 = double_to_fp64(2.0);
    fp64 r64 = fp64_div(v1_64, v2_64);
    TEST_ASSERT(double_almost_equal(fp64_to_double(r64), 2.5));

    fpu32 v1_u32 = double_to_fpu32(5.0);
    fpu32 v2_u32 = double_to_fpu32(2.0);
    fpu32 ru32 = fpu32_div(v1_u32, v2_u32);
    TEST_ASSERT(double_almost_equal(fpu32_to_double(ru32), 2.5));

    fpu64 v1_u64 = double_to_fpu64(5.0);
    fpu64 v2_u64 = double_to_fpu64(2.0);
    fpu64 ru64 = fpu64_div(v1_u64, v2_u64);
    TEST_ASSERT(double_almost_equal(fpu64_to_double(ru64), 2.5));
TEST_CASE_END

/*! array that contains pointers to all the test functions; used to run all of the tests. */
static test_function test_functions[] = {
    &fp32_double_conversions,
    &fp64_double_conversions,
    &fpu32_double_conversions,
    &fpu64_double_conversions,
    &fp32_float_conversions,
    &fp64_float_conversions,
    &fpu32_float_conversions,
    &fpu64_float_conversions,
    &fp32_int_conversions,
    &fp64_int_conversions,
    &fpu32_int_conversions,
    &fpu64_int_conversions,
    &fractions,
    &whole_parts,
    &add,
    &sub,
    &mul,
    &division
};

/*! implementation function that runs all the tests 
 *  will abort upon the first test failure.
 *  returns the test_result indicating which expression caused an assertion violation
 *  in which test function
 *  if the failure_expression data member in the test_result struct returned is NULL
 *  all tests were executed successfully.
**/
static struct test_result
all_tests(void)
{
    for (size_t i = 0U; i < COUNT_OF(test_functions); ++i) {
        TEST_RUN(test_functions[i]); /* run each function; will return on failure */
    }

    /* if we got here all tests were executed successfully */
    struct test_result return_value = { NULL, NULL };
    return return_value;
}

bool
run_all_tests(void)
{
    struct test_result result = all_tests(); /* run all of the tests */
    if (result.failure_expression != NULL)   /* a test failed */ 
    {
        /* print which test failed and on which expression if failed */
        printf_stderr(
            "!!!TEST FAILED!!! Test: \"%s\""
            " expression: \"%s\" was false\n",
            result.test_fun,
            result.failure_expression
        );
    }
    else /* if result.failure_expression is NULL -> all tests were successfull executed. */
    {
        printf_stderr("ALL TESTS PASSED\n"); /* print a message indicating that all tests passed */
    }

    /* print the amount of tests that ran */
    printf_stderr("Tests run: %d\n", tests_run);

    /* return a value that evaluates to true if all tests were successfully executed.
     * otherwise (if any test failed) -> return a value that evaluates to false.   
    **/
    return result.failure_expression == NULL;
}
