#ifndef RIOTNERF_TESTS_H
#define RIOTNERF_TESTS_H
#include <stdbool.h> /* bool, false */
#include <stdlib.h> /* NULL */

/*! implementation macro of STRINGIFY; not to be used directly */
#define STRINGIFY_IMPL(x)   #x

/*! turns x into a string literal */
#define STRINGIFY(x)        STRINGIFY_IMPL(x)

/*! struct used for the result of a test
 *  if failure_expression != NULL the test failed.
 *  if the test failed failure_expression 
 *  will contain the text of the expression that was tested and evaluated to false.
 *  test_fun will contain the text of the identifier of the associated test function.
**/
struct test_result
{
    /* the expression that made the test fail; NULL if test was successfull */
    char const *failure_expression;

    /* the test function that this is the result of */
    char const *test_fun;
};

/*! macro that will test the expression passed in. To be used in test functions. */
#define TEST_ASSERT(test) \
    do \
    { \
        struct test_result ret = { NULL, NULL }; \
        ret.test_fun = __FUNCTION__; \
        if (!(test)) \
        { \
            ret.failure_expression = STRINGIFY(test); \
            return ret; \
        } \
    } while (false) 

/*! runs a test function; to be used in the function that will run all the tests. */
#define TEST_RUN(test) \
    do \
    { \
        struct test_result ret = test(); \
        ++tests_run; \
        if (ret.failure_expression != NULL) \
        { \
            return ret; \
        } \
    } while (false)

/*! macro to introduce a test case. 
 *  will declare a function called "identifier"
 *  place TEST_ASSERT statements within the test case.
 *  a test case must end with TEST_CASE_END
**/
#define TEST_CASE(identifier) \
    static struct test_result \
    identifier(void) \
    { \

/*! the end of a test case */
#define TEST_CASE_END \
        struct test_result test_result_thing = { NULL, NULL }; \
        return test_result_thing; \
    }

/*! an extern int that is used to count the amount of tests that were executed.
 *  must be defined as int tests_run = 0 in a source file.
**/
extern int tests_run;

/*! function to be called from main to run the tests.
 *  will be the entry point for the test running stuff.
**/
bool
run_all_tests(void);

#endif /* RIOTNERF_TESTS_H */
