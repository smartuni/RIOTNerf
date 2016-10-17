/*
 * Copyright (C) 2015 Eistec AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Test for servo driver
 *
 * This test initializes the given servo device and moves it between
 * 1.000 -- 2.000 ms, roughly -/+ 90 degrees from the middle position if the
 * connected servo is a standard RC servo.
 *
 * @author      Joakim Nohlgård <joakim.nohlgard@eistec.se>
 *
 * @}
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
#define STEP_UPPER_BOUND (2000U)

/* Step size that we move per WAIT us */
#define STEP             (1U)

/* Sleep time between updates, no need to update the servo position more than
 * once per cycle */
#define WAIT             (2000U)

#define NEUTRAL_SERVO_SCALE (1)

static servo_t servo;
static servo_t servo2;

int main(void)
{
    int res;
    int res2;
    int pos = (STEP_LOWER_BOUND + STEP_UPPER_BOUND) / 2;
    int step = STEP;

    puts("\nRIOT RC servo test");
    puts("Connect an RC servo or scope to PWM_0 channel 0 to see anything");

    res = servo_init(&servo, DEV, CHANNEL, SERVO_MIN, SERVO_MAX);
    servo.scale_nom = NEUTRAL_SERVO_SCALE;
    servo.scale_den = NEUTRAL_SERVO_SCALE;
    if (res < 0) {
        puts("Errors while initializing servo 1");
        return -1;
    }
    puts("Servo 1 initialized.");

    res2 = servo_init(&servo2, DEV, CHANNEL2, SERVO_MIN, SERVO_MAX);
    servo2.scale_nom = NEUTRAL_SERVO_SCALE;
    servo2.scale_den = NEUTRAL_SERVO_SCALE;
    if (res2 < 0) {
        puts("Errors while initializing servo 2");
        return -1;
    }
    puts("Servo 2 initialized.");

    while (1) {
        servo_set(&servo, pos);
        servo_set(&servo2, pos);

        pos += step;
        if (pos <= STEP_LOWER_BOUND || pos >= STEP_UPPER_BOUND) {
            step = -step;
        }

        xtimer_usleep(WAIT);
    }

    return 0;
}
