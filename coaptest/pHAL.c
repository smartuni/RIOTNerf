/*
 * pseudoHAL for the servo side
**/

/*! \def SERVO_FREQUENCY
 *  \brief Macro to predefine the servo frequency to be used
 *         in the corresponding RIOT headers.
 *  \see SERVO_RESOLUTION
 *  \see NEUTRAL_SERVO_SCALE
 *
 *  The frequency's unit is Hz.
 *  Used to be able to use the full range of the servos employed.
**/
#define SERVO_FREQUENCY                    (48U)  /* request 48 Hz */

/*! \def SERVO_RESOLUTION
 *  \brief Macro to predefine the servo resolution to be used
 *         in the corresponding RIOT headers.
 *  \see SERVO_FREQUENCY
 *  \see NEUTRAL_SERVO_SCALE
 *
 *  Used to be able to use the full range of the servos employed.
**/
#define SERVO_RESOLUTION                   (10000U)

#include <stdio.h> /* puts, printf */
#include <stdint.h> /* uint32_t */
#include <cpu.h>
#include <board.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include <periph/pwm.h>
#include <servo.h>
#include "pHAL.h"

/*! \def DEV
 *  \brief The device to be used: PWM.
 *  \see CHANNEL
 *  \see CHANNEL2
 *  \see pHAL_init
 *
 *  Used in calls to servo_init in conjunction with CHANNEL or CHANNEL2
 *  in order to initialize a servo in pHAL_init.
**/
#define DEV                                 (PWM_0)

/*! \def CHANNEL 
 *  \brief The channel for the horizontal servo.
 *  \see DEV
 *  \see pHAL_init
 *
 *  Used in calls to servo_init in conjunction with DEV
 *  in order to initialize the horizontal servo in pHAL_init.
**/
#define CHANNEL                             (0)

/*! \def CHANNEL2
 *  \brief The channel for the vertical servo.
 *  \see DEV
 *  \see pHAL_init
 *
 *  Used in calls to servo_init in conjunction with DEV
 *  in order to initialize the vertical servo in pHAL_init.
**/
#define CHANNEL2                            (1)

/*! \def SERVO_MIN
 *  \brief the minimum position of a servo
 *  \see SERVO_MAX
**/
#define SERVO_MIN                           (500U)

/*! \def SERVO_MAX
 *  \brief the maximum position of a servo
 *  \see SERVO_MIN
**/
#define SERVO_MAX                           (2000U)

/* \def NEUTRAL_SERVO_SCALE
 * \brief value to set the scale nominator and scale demoninator
 *        of the servos to.
 * \see SERVO_FREQUENCY
 * \see SERVO_RESOLUTION
 *
 * Used to be able to use the full range of the servos employed.
**/
#define NEUTRAL_SERVO_SCALE                 (1)

/*! \def PASS_TO_MACRO(target_macro, ...)
 *  \brief passes arguments to the target_macro.
 *  \param target_macro the macro to pass the following arguments to.
 *
 *  Can be used to pass the expansion of a macro to target_macro
 *  rather than the macro itself, by adding an extra level of indirection. 
 *  For example GPIO_PIN(LASER_PORT) will not compile
 *  as GPIO_PIN expects two arguments.
 *  However PASS_TO_MACRO(GPIO_PIN, LASER_PORT) will compile as it will first expand
 *  to GPIO_PIN(1, 3) and then proceed to expand that.
**/
#define PASS_TO_MACRO(target_macro, ...)    target_macro(__VA_ARGS__)

/*! \def LASER_PORT
 *  \brief the port to use for the laser
**/
#define LASER_PORT                          1, 3

/*! \def MINIMUM_STEP_COUNT
 *  \brief the minimum amount of steps to perform in the stepn functions.
 *  \see stepn_d
 *  \see stepn_l
 *  \see stepn_r
 *  \see stepn_u
 *
 *  For steps lower than MINIMUM_STEP_COUNT the stepn function will do nothing.
**/
#define MINIMUM_STEP_COUNT                  (1)

static servo_t servo_h; /*!< the horizontal servo */
static servo_t servo_v; /*!< the vertical servo */

static int session_pos_h = 0; /*!< the position of the horizontal servo */
static int session_pos_v = 0; /*!< the position of the vertical servo */

/*! \brief in_boundary checks whether a servo position is within the
 *         allowable range.
 *  \param pos the position to check.
 *  \return returns a non-zero value if pos is within the allowable range
 *          of servo positions. returns 0 otherwise.
**/
static int in_boundary(int pos);

/*! \brief get_pos converts an angle in degrees to a servo position
 *  \param angle the angle to convert to a servo position.
 *         must be within -90 and +90.         
 *  \return the servo position corresponding to the angle passed in
 *          the result will be in the range of [500..2000]
 *  \warning Do not call with angles < -90 or > 90.
**/
static int get_pos(int angle);

/*! \brief create_servo Creates a servo.
 *  \param servo pointer to the servo to create.
 *  \param channel the channel to use when initializing the servo.__a
 *         pass CHANNEL to create the horizontal servo.
 *         pass CHANNEL2 to create the vertical servo.
 *  \return on failure return a value < 0.
 *  \see CHANNEL
 *  \see CHANNEL2
 *  \see pHAL_init
 *  \warning Undefined behavior if servo is NULL,
 *           or if channel is not CHANNEL or CHANNEL2.
 *
 *  Used in pHAL_init to initialize the servos.
**/
static int create_servo(servo_t *servo, int channel) {
    int result = servo_init(servo, DEV, channel, SERVO_MIN, SERVO_MAX);

    /* deactivate servo scaling to be able to use the full servo range */
    servo->scale_nom = NEUTRAL_SERVO_SCALE;
    servo->scale_den = NEUTRAL_SERVO_SCALE;
    return result;
}

int pHAL_init(void) {
    static int res; /* will hold the error code returned by create_servo */
    
    puts("Initializing pHAL servo");

    /* initialize the laser */
    if (gpio_init(PASS_TO_MACRO(GPIO_PIN, LASER_PORT), GPIO_OUT) < 0) {
        printf("Error to initialize GPIO_PIN\n");
        return PHAL_INIT_LASER_ERROR;
    }
    
    /* initialize the horizontal servo */
    res = create_servo(&servo_h, CHANNEL);

    if (res < 0) {
        puts("Errors while initializing servo 1");
        return PHAL_INIT_SERVO1_ERROR;
    }
    puts("Servo H initialized.");


    /* initialize the vertical servo */
    res = create_servo(&servo_v, CHANNEL2);
    
    if (res < 0) {
        puts("Errors while initializing servo 2");
        return PHAL_INIT_SERVO2_ERROR;
    }

    puts("Servo V initialized.");

    /* set both servos to the lowest position possible to start out */
    session_pos_h = SERVO_MIN;
    session_pos_v = SERVO_MAX;

    servo_set(&servo_h, session_pos_h);
    servo_set(&servo_v, session_pos_v);

    printf("Servos init, h: %d, v: %d\n", session_pos_h, session_pos_v);
    return 0;
}

/* get angle interpretation and turn into servo conform value */
void set_h(int angle) {
    session_pos_h = get_pos(angle);
    servo_set(&servo_h, session_pos_h);
}

/* get angle interpretation and turn into servo conform value */
void set_v(int angle) {
    session_pos_v = get_pos(angle);
    servo_set(&servo_v, session_pos_v);
}

void sethv(int h, int v) {
    session_pos_h = h;
    session_pos_v = v;
    servo_set(&servo_h, session_pos_h);
    servo_set(&servo_v, session_pos_v);
}

/* stepsize in this V in degree, might be changed to a pwm value */
void step_l(void) {
    printf("in %s, sessionval h: %d\n", __FUNCTION__, session_pos_h);
    session_pos_h += STEP_SIZE;

    if (in_boundary(session_pos_h)) {
        printf("Managed to get into %s, new sessh val: %d\n", __FUNCTION__,
               session_pos_h);
        servo_set(&servo_h, session_pos_h);
    } else {
        session_pos_h = SERVO_MAX;
        servo_set(&servo_h, session_pos_h);
    }
}

/* stepsize in this V in degree, might be changed to a pwm value */
void step_r(void) {
    printf("in %s, sessionval h: %d\n", __FUNCTION__, session_pos_h);
    session_pos_h -= STEP_SIZE;

    if (in_boundary(session_pos_h)) {
        servo_set(&servo_h, session_pos_h);
    } else {
        session_pos_h = SERVO_MIN;
        servo_set(&servo_h, session_pos_h);
    }
}

/* stepsize in this V in degree, might be changed to a pwm value */
void step_u(void) {
    printf("in %s, sessionval v: %d\n", __FUNCTION__, session_pos_v);

    session_pos_v -= STEP_SIZE;

    if (in_boundary(session_pos_v)) {
        servo_set(&servo_v, session_pos_v);
    } else {
        session_pos_v = SERVO_MIN;
        servo_set(&servo_v, session_pos_v);
    }
}

/* stepsize in this V in degree, might be changed to a pwm value */
void step_d(void) {
    printf("in %s, sessionval v: %d\n", __FUNCTION__, session_pos_v);
    session_pos_v += STEP_SIZE;
    if (in_boundary(session_pos_v)) {
        servo_set(&servo_v, session_pos_v);
    } else {
        session_pos_v = SERVO_MAX;
        servo_set(&servo_v, session_pos_v);
    }
}

void stepn_l(int n) {
    if (n < MINIMUM_STEP_COUNT) {
        return;
    }

    printf("in %s, sessionval h: %d\n", __FUNCTION__, session_pos_h);
    session_pos_h += STEP_SIZE * n;

    if (in_boundary(session_pos_h)) {
        printf("Managed to get into %s, new sessh val: %d\n",
               __FUNCTION__, session_pos_h);
        servo_set(&servo_h, session_pos_h);
    } else {
        session_pos_h = UPPER_BOUND;
        servo_set(&servo_h, session_pos_h);
    }
}

void stepn_r(int n) {
    if (n < MINIMUM_STEP_COUNT) {
        return;
    }

    printf("in %s, sessionval h: %d\n", __FUNCTION__, session_pos_h);
    session_pos_h -= STEP_SIZE * n;

    if (in_boundary(session_pos_h)) {
        servo_set(&servo_h, session_pos_h);
    } else {
        session_pos_h = LOWER_BOUND;
        servo_set(&servo_h, session_pos_h);
    }
}

void stepn_u(int n) {
    if (n < MINIMUM_STEP_COUNT) {
        return;
    }

    printf("in %s, sessionval v: %d\n", __FUNCTION__, session_pos_v);

    session_pos_v -= STEP_SIZE * n;

    if (in_boundary(session_pos_v)) {
        servo_set(&servo_v, session_pos_v );
    } else {
        session_pos_v = LOWER_BOUND;
        servo_set(&servo_v, session_pos_v);
    }
}
 
void stepn_d(int n) {
    if (n < MINIMUM_STEP_COUNT) {
        return;
    }

    printf("in %s, sessionval v: %d\n", __FUNCTION__, session_pos_v);
    session_pos_v += STEP_SIZE * n;
    if (in_boundary(session_pos_v)) {
        servo_set(&servo_v, session_pos_v );
    } else {
        session_pos_v = UPPER_BOUND;
        servo_set(&servo_v, session_pos_v);
    }
}

void laser_on(void) {
    gpio_set(PASS_TO_MACRO(GPIO_PIN, LASER_PORT));
}

void laser_off(void) {
    gpio_clear(PASS_TO_MACRO(GPIO_PIN, LASER_PORT));
}

void laser_toggle(void) {
    gpio_toggle(PASS_TO_MACRO(GPIO_PIN, LASER_PORT));
}

void align_cntr(void) {
    align_h_cntr();
    align_v_cntr();
}

void align_h_cntr(void) {
    /* center the horizontal servo by setting it to 0 degrees */
    set_h(0);
}

void align_v_cntr(void) {
    /* center the vertical servo by setting it to 0 degrees */
    set_v(0);
}

/* get the pwm val in here, not angle! */
int in_boundary(int pos) {
    static const int lowerBound = (int) SERVO_MIN;
    static const int upperBound = (int) SERVO_MAX;

    printf("in %s, pos is: %d\n", __FUNCTION__, pos);
    return (pos >= lowerBound && pos <= upperBound);
}

/* user input -> servo input conversion
 * input expected: [-90°..90°]
 * return: [500..2000]
**/
static int get_pos(int angle) {
    /* offset used to turn an angle into a positive angle
       used to convert the input angle in the parameter which is in the range
       of [-90..90] to [0..180]
    */
    static const int angleToPosOffset = 90;

    /* value to be divided by the maxPosAngle to get the 
       percentage of one degree.
    */
    static const int percent = 100;

    /* 16 bits.
       used to shift things around so that precision isn't lost
    */
    static const int u32half = sizeof(uint32_t) / 2;

    /* the maximum positive angle */
    static const int maxPosAngle = 180;

    /* used to convert the percentage of a positive angle to a servo position
       the resulting servo position still has to be added to posOffset so 
       that it is in range [500..2000].
    */
    static const int angleToPosMultiplier = 15;

    /* offset used to convert a position of [0..1500]
       to one within [500..2000].
    */
    static const int posOffset = 500;

    /* the percentage of one degree shifted rightward by 16 bits */
    static const uint32_t oneDegreePercentShifted =
        (percent << u32half) / maxPosAngle;

    /* turn the angle into a positive angle ranged [0..180] */
    angle += angleToPosOffset;

    /* calculate the servo position
       it still needs to be shifted back and converted into
       range [500..2000]
    */
    uint32_t pos = (angle * oneDegreePercentShifted) * angleToPosMultiplier;

    /* shift it back to the low bits */
    pos >>= u32half;

    /* add the offset, so that it's in range [500..2000] */
    pos += posOffset;

    return pos; /* returns pos */
}
