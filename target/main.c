/*
 * Copyright 2015 Ludwig Knüpfer
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Test application for the ISL29125 RGB light sensor
 *
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 * @author      Martin Heusmann <martin.heusmann@haw-hamburg.de>
 *
 * @}
 */

#ifndef TEST_ISL29125_I2C
#error "TEST_ISL29125_I2C not defined"
#endif

#ifndef TEST_ISL29125_IRQ_PIN
#error "ISL29125_IRQ_PIN not defined"
#endif

#include <stdio.h> /* puts, printf */
#include <string.h> /* memset */
#include <stdint.h> /* uint16_t */
#include "xtimer.h"
#include "isl29125.h"
#include "periph/gpio.h"

/*! \def SLEEP
 *  \brief Time to sleep
 *
 *  Sleep for 250 milliseconds.
 *  Used as an argument to xtimer_usleep.
**/
#define SLEEP                               (250U * 1000U)

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

/*! \def RN_COUNTOF(array)
 *  \brief macro to calculate the count of elements of an array
 *  \param array the array to calculate the amount of elements for
 *  \warning Only pass arrays into the parameter,
 *           not pointers to the 0th element of an array.
 *           Beware of array to pointer decay.
**/
#define RN_COUNTOF(array)	                ((sizeof((array))) / (sizeof(*(array))))

#define ENABLE_LED_FUNCTIONS                0 /* disabled by default */

/*! \brief cb the callback function for the external interrupt
 *  \param arg the pin of the external interrupt of type void *, must be cast to int.
**/
void cb(void *arg);

/*! \brief init_ext_int Creates an external interrupt
 *  \param po The GPIO port to use
 *  \param pi The GPIO pin to use, associated with the GPIO port.
 *  \param mode The mode.
 *         Possible values are:
 *         GPIO_IN    - input w/o pull R
 *         GPIO_IN_PD - input with pull-down
 *         GPIO_IN_PU - input with pull-up
 *         GPIO_OUT   - push-pull output
 *         GPIO_OD 	  - open-drain w/o pull R
 *         GPIO_OD_PU - open-drain with pull-up
 *         GPIO_IN    - configure as input without pull resistor
 *         GPIO_IN_PD - configure as input with pull-down resistor
 *         GPIO_IN_PU - configure as input with pull-up resistor
 *         GPIO_OUT   - configure as output in push-pull mode
 *         GPIO_OD    - configure as output in open-drain mode without pull resistor
 *         GPIO_OD_PU - configure as output in open-drain mode with pull resistor enable
 *  \param flank Trigger the external interrupt on a rising or falling flank.
 *         Possible values: GPIO_FALLING, GPIO_RISING, GPIO_BOTH
 *  \return 1 on error; 0 on success.
**/
int init_ext_int(int po, int pi, gpio_mode_t mode, gpio_flank_t flank);

#if ENABLE_LED_FUNCTIONS
/*! \brief led_on Turns the LED on.
 *  \see led_off
**/
void led_on(void);

/*! \brief led_off Turns the LED off.
 *  \see led_on
**/
void led_off(void);
#endif

static isl29125_t dev; /*!< the device */
static isl29125_rgb_t data; /*!< the data of the device */

int main(void) {
 /* isl29125_t dev;
    isl29125_rgb_t data; */
    
    color_rgb_t data8bit;
    memset(&data, 0, sizeof(data));

    /* initialize the GPIO */
    if (gpio_init(PASS_TO_MACRO(GPIO_PIN, LASER_PORT), GPIO_OUT) < 0) {

int main(void)
{
        /*isl29125_t dev;
        isl29125_rgb_t data;*/
    color_rgb_t data8bit;
    memset(&data, 0x00, sizeof(data));

    if (gpio_init(GPIO_PIN(1, 3), GPIO_OUT) < 0) {
        printf("Error to initialize GPIO_PIN\n");
    }

    /* Parameters for testing, change if needed. */
    const uint16_t lower_threshold = 0;
    const uint16_t higher_threshold = 1000;
    const int po = 0;             /* GPIO_PIN(0, 22) = PA22 */
    const int pi = 22;
    const gpio_mode_t mode = GPIO_IN;
    const gpio_flank_t flank = GPIO_FALLING;

    /* create the external interrupt */
    init_ext_int(po, pi, mode, flank);

    puts("ISL29125 light sensor test application\n");
    printf("Initializing ISL29125 sensor at I2C_%i... ", TEST_ISL29125_I2C);
    if (isl29125_init(&dev, TEST_ISL29125_I2C, TEST_ISL29125_IRQ_PIN,
                      ISL29125_MODE_R, ISL29125_RANGE_10K,
                      ISL29125_RESOLUTION_16, ISL29125_INTERRUPT_STATUS_RED,
                      ISL29125_INTERRUPT_PERSIST_1, ISL29125_INTERRUPT_CONV_DIS,
                      lower_threshold, higher_threshold) == 0) {
        puts("[OK]\n");
    } else {
        puts("[Failed]");
        return 1;
    }

    /* try out some modes */
    static const isl29125_mode_t modes[] = {
        ISL29125_MODE_DOWN, ISL29125_MODE_STANDBY, ISL29125_MODE_RGB,
        ISL29125_MODE_R, ISL29125_MODE_G, ISL29125_MODE_B,
        ISL29125_MODE_RG, ISL29125_MODE_GB
    };

    static const char *mode_names[] = {
        "ISL29125_MODE_DOWN", "ISL29125_MODE_STANDBY", "ISL29125_MODE_RGB",
        "ISL29125_MODE_R", "ISL29125_MODE_G", "ISL29125_MODE_B",
        "ISL29125_MODE_RG", "ISL29125_MODE_GB"
    };

    for (int i = 0; i < RN_COUNTOF(modes); i++) {
        printf("Setting mode %s\n", mode_names[i]);
        isl29125_set_mode(&dev, modes[i]);
        xtimer_usleep(SLEEP);
        isl29125_read_rgb_color(&dev, &data8bit);
        printf("RGB value: (%3i / %3i / %3i) 8 bit\n",
               data8bit.r, data8bit.g, data8bit.b);
    }

    puts("Resetting mode to RGB and reading continuously");
    isl29125_set_mode(&dev, ISL29125_MODE_RGB);
    initcoaptarget("fe80::5841:1644:2407:f2c2", "5683");
    xtimer_usleep(SLEEP);
    for (;;) {
#if 0
        printf("...\n");
        xtimer_sleep(1);
        isl29125_read_rgb_lux(&dev, &data);
        printf("RGB value: (%5i / %5i / %5i) lux\n",
               (int)data.red, (int)data.green, (int)data.blue);
        xtimer_usleep(SLEEP);
        printf("IRQ-Status: %i \n", isl29125_read_irq_status(&dev));
#endif /* 0 */
    }

    return 0;
}

void cb(void *arg) {
    printf("INT: external interrupt from pin %i\n", (int) arg);
    isl29125_read_rgb_lux(&dev, &data);
    printf("RGB value: (%5i / %5i / %5i) lux\n",
           (int) data.red, (int) data.green, (int) data.blue);
    printf("IRQ-Status: %i \n", isl29125_read_irq_status(&dev));
    sendGotHit();
    xtimer_usleep(SLEEP);
 /* led_on();
    xtimer_sleep(10);
    led_off(); */
}

int init_ext_int(int po, int pi, gpio_mode_t mode, gpio_flank_t flank) {
    if (gpio_init_int(GPIO_PIN(po, pi), mode, flank, &cb, (void *) pi) < 0) {
        printf("error: init_int of GPIO_PIN(%i, %i) failed\n", po, pi);
        return 1;
    }
    printf("GPIO_PIN(%i, %i) successfully initialized as ext int\n", po, pi);

    return 0;
}

#if ENABLE_LED_FUNCTIONS
void led_on(void) {
    gpio_set(PASS_TO_MACRO(GPIO_PIN, LASER_PORT));
}

void led_off(void) {
    gpio_clear(PASS_TO_MACRO(GPIO_PIN, LASER_PORT));
}
      
#endif
