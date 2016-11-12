/*
* pseudoHAL for the servo side
*/
#define SERVO_FREQUENCY  (48U)  /* request 48 Hz */
#define SERVO_RESOLUTION (10000U)

#include <stdio.h>

#include <cpu.h>
#include <board.h>
#include <xtimer.h>
#include <periph/pwm.h>
#include <servo.h>
#include "pHAL.h"

#define DEV         PWM_0
#define CHANNEL     0
#define CHANNEL2    1

#define SERVO_MIN        (500U)
#define SERVO_MAX        (2000U)

/* these are defined outside the limits of the servo_init min/max parameters above */
/* we will test the clamping functionality of the servo_set function. */
#define LOWER_BOUND (500U)
#define UPPER_BOUND (2000U)

/* Step size that we move per WAIT us */
#define STEP             (1U)

/* Sleep time between updates, no need to update the servo position more than
 * once per cycle */
#define WAIT             (2000U)

#define MED ( (LOWER_BOUND + UPPER_BOUND) / 2 )

#define NEUTRAL_SERVO_SCALE (1)

static int res;
static int res2;
//static int pos_x = MED;
//static int pos_y = MED;
//static int step_x = STEP;
//static int step_y = STEP;

static servo_t servo_h;
static servo_t servo_v;

static int session_pos_h = 0;
static int session_pos_v = 0;

static int get_pos(int);
int in_boundary(int pos);

int pHAL_init(void)
{

    puts("Initializing pHAL servo");

    res = servo_init(&servo_h, DEV, CHANNEL, SERVO_MIN, SERVO_MAX);
    servo_h.scale_nom = NEUTRAL_SERVO_SCALE;
    servo_h.scale_den = NEUTRAL_SERVO_SCALE;
    if (res < 0) {
        puts("Errors while initializing servo 1");
        return -1;
    }
    puts("Servo H initialized.");

    res2 = servo_init(&servo_v, DEV, CHANNEL2, SERVO_MIN, SERVO_MAX);
    servo_v.scale_nom = NEUTRAL_SERVO_SCALE;
    servo_v.scale_den = NEUTRAL_SERVO_SCALE;
    if (res2 < 0) {
        puts("Errors while initializing servo 2");
        return -1;
    }
    puts("Servo V initialized.");

    session_pos_h = LOWER_BOUND;
    session_pos_v = LOWER_BOUND;

    servo_set(&servo_h, session_pos_h);
    servo_set(&servo_v, session_pos_v);

    printf("Servos init, h: %d, v: %d\n", session_pos_h, session_pos_v);
    return 0;

}

/*get angle interpretation and turn into servo conform value*/
void set_h(int angle)
{
    session_pos_h = get_pos(angle);
    servo_set(&servo_h, session_pos_h);
}

/*get angle interpretation and turn into servo conform value*/
void set_v(int angle)
{
    session_pos_v = get_pos(angle);
    servo_set(&servo_v, session_pos_v);
}

/*stepsize in this V in degree, might be changed to a pwm value*/
void step_l(void)
{
    printf("in stepl, sessionval h: %d\n", session_pos_h);
    session_pos_h += STEP_SIZE;

    if (in_boundary(session_pos_h)) {
        printf("Managed to get into step_l, new sessh val: %d\n",
               session_pos_h);
        servo_set(&servo_h, session_pos_h);
    } else {
        session_pos_h = UPPER_BOUND;
        servo_set(&servo_h, session_pos_h);
    }
}

/*stepsize in this V in degree, might be changed to a pwm value*/
void step_r(void)
{
    printf("in stepr, sessionval h: %d\n", session_pos_h);
    session_pos_h -= STEP_SIZE;

    if (in_boundary(session_pos_h)) {
        servo_set(&servo_h, session_pos_h);
    } else {
        session_pos_h = LOWER_BOUND;
        servo_set(&servo_h, session_pos_h);
    }
}

/*stepsize in this V in degree, might be changed to a pwm value*/
void step_u(void)
{
    printf("in stepu, sessionval v: %d\n", session_pos_v);

    session_pos_v -= STEP_SIZE;

    if (in_boundary(session_pos_v)) {
        servo_set(&servo_v, session_pos_v);
    } else {
        session_pos_v = LOWER_BOUND;
        servo_set(&servo_v, session_pos_v);
    }
}

/*stepsize in this V in degree, might be changed to a pwm value*/
void step_d(void)
{
    printf("in stepd, sessionval v: %d\n", session_pos_v);
    session_pos_v += STEP_SIZE;
    if (in_boundary(session_pos_v)) {
        servo_set(&servo_v, session_pos_v);
    } else {
        session_pos_v = UPPER_BOUND;
        servo_set(&servo_v, session_pos_v);
    }
}

void allign_cntr(void)
{
    set_h(0);
    set_v(0);
}

void allign_h_cntr(void)
{
    set_h(0);
}

void allign_v_cntr(void)
{
    set_v(0);
}


/*get the pwm val in here, not angle!*/
int in_boundary(int pos)
{
    printf("in boundary check, pos is: %d\n", pos);
    return (pos >= (int) LOWER_BOUND && pos <= (int) UPPER_BOUND);
}


/*  user input -> servo input conversion
    input expected: {-90°...90°}
    return: {500...2000}*/
static int get_pos(int angle)
{

    angle += 90;
    uint32_t PERCENT_ANGLE = (100 << 16) / 180;
    uint32_t pos = (angle * PERCENT_ANGLE) * 15;
    pos = pos >> 16;
    pos = pos + 500;
    return pos;                 // returns pos
}
