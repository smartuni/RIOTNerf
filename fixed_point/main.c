#include "tests.h"  /* run_all_tests */
#include <stdlib.h> /* EXIT_SUCCESS */
#include <stdio.h>  /* printf */
#include <limits.h> /* CHAR_BIT */

#if CHAR_BIT != 8
#   error "This application assumes bytes are 8 bit large"
#endif /* CHAR_BIT != 8 */

/*! macro used to mark a function parameter as unused; used to supress compiler warnings */
#define UNUSED(arg) ((void) (arg))

/*! macro that expands to "true" or "false" 
 *  depending on whether the boolean_expression is true or false;
 *  used to easily be able to print boolean values.
**/
#define BOOLALPHA(boolean_expression) \
    ((boolean_expression) ? "true" : "false")

/*! the main function - the entry point of this application */
int main(int argc, char *argv[]) 
{
    UNUSED(argc);
    UNUSED(argv);

    bool retV = run_all_tests(); /* will be true if all tests were successful */

    printf("Test run result: %s\n", BOOLALPHA(retV));

    return EXIT_SUCCESS; /* exit the application */
}
