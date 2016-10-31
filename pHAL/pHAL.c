/*
* pseudoHAL for the servo side
*/
#define SERVO_FREQUENCY  (48U) /* request 48 Hz */
#define SERVO_RESOLUTION (10000U)

#include <stdio.h>

#include "cpu.h"
#include "board.h"
#include "xtimer.h"
#include "periph/pwm.h"
#include "servo.h"



#define DEV         PWM_0
#define CHANNEL     0
#define CHANNEL2    1

#define SERVO_MIN        (500U)
#define SERVO_MAX        (2000U)

/* these are defined outside the limits of the servo_init min/max parameters above */
/* we will test the clamping functionality of the servo_set function. */
#define STEP_LOWER_BOUND (500U)
#define STEP_UPPER_BOUND (2000U )



/* Step size that we move per WAIT us */
#define STEP             (1U)

/* Sleep time between updates, no need to update the servo position more than
 * once per cycle */
#define WAIT             (2000U)

#define MED ( (STEP_LOWER_BOUND + STEP_UPPER_BOUND) / 2 )

#define NEUTRAL_SERVO_SCALE (1)

static int res;
static int res2;
//static int pos_x = MED;
//static int pos_y = MED;
//static int step_x = STEP;
//static int step_y = STEP;


static servo_t servo_x;
static servo_t servo_y;

static int get_pos(int );

int pHAL_init(void){
    puts("Initializing pHAL servo");

    res = servo_init(&servo_x, DEV, CHANNEL, SERVO_MIN, SERVO_MAX);
    servo_x.scale_nom = NEUTRAL_SERVO_SCALE;
    servo_x.scale_den = NEUTRAL_SERVO_SCALE;
    if (res < 0) {
        puts("Errors while initializing servo 1");
        return -1;
    }
    puts("Servo X initialized.");

    res2 = servo_init(&servo_y, DEV, CHANNEL2, SERVO_MIN, SERVO_MAX);
    servo_y.scale_nom = NEUTRAL_SERVO_SCALE;
    servo_y.scale_den = NEUTRAL_SERVO_SCALE;
    if (res2 < 0) {
        puts("Errors while initializing servo 2");
        return -1;
    }
    puts("Servo Y initialized.");
    return 0;

}

void move_x(int angle){
    servo_set(&servo_x, get_pos(angle));
}


void move_y(int angle){
    servo_set(&servo_y, get_pos(angle));
}

/**/
int in_boundary(int pos){
    return ( pos >= STEP_LOWER_BOUND && pos <= STEP_UPPER_BOUND );
}

/*  user input -> servo input conversion
    input expected: {-90°...90°}
    return: {500...2000}*/
static int get_pos(int angle){
    printf("in pHAL, get_pos\n");
    angle += 90;

    printf("before offset, angle: %d\n", angle);
    printf("after offset, angle: %d\n", angle);
    uint32_t PERCENT_ANGLE = (100 << 16) / 180;
    printf("PERCENT_ANGLE: %lu\n", PERCENT_ANGLE);
    uint32_t pos = (angle  * PERCENT_ANGLE) * 15;
    printf("FInal pos calced (missing 500): %lu\n", pos);
    pos = pos >> 16;
    pos = pos + 500;

    printf("position scaled: %lu \n", pos);
    return pos; // returns pos
}
