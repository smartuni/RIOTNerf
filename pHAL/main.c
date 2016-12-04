#include <stdio.h> /* puts, printf */
#include <stdlib.h> /* atoi */
#include <shell.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include "pHAL.h"

/*! \def RN_UNUSED(arg)
 *  \brief Macro to mark a parameter as unused.
 *  \param arg The parameter to mark as unused.
 *
 *  Used to suppress compiler warnings, because of unused function parameters.
**/
#define RN_UNUSED(arg)  ((void) (arg))

static void cb(void *arg) {
    printf("INT: external interrupt from pin %i\n", (int) arg);
}

static int init_pin(int argc, char **argv, gpio_mode_t mode) {
    int po, pi;

    if (argc < 3) {
        printf("usage: %s <port> <pin>\n", argv[0]);
        return 1;
    }

    po = atoi(argv[1]);
    pi = atoi(argv[2]);

    if (gpio_init(GPIO_PIN(po, pi), mode) < 0) {
        printf("Error to initialize GPIO_PIN(%i, %02i)\n", po, pi);
        return 1;
    }

    return 0;
}

static int init_out(int argc, char **argv) {
    return init_pin(argc, argv, GPIO_OUT);
}

static int init_in(int argc, char **argv) {
    return init_pin(argc, argv, GPIO_IN);
}

static int init_in_pu(int argc, char **argv) {
    return init_pin(argc, argv, GPIO_IN_PU);
}

static int init_in_pd(int argc, char **argv) {
    return init_pin(argc, argv, GPIO_IN_PD);
}

static int init_od(int argc, char **argv) {
    return init_pin(argc, argv, GPIO_OD);
}

static int init_od_pu(int argc, char **argv) {
    return init_pin(argc, argv, GPIO_OD_PU);
}

static int init_int(int argc, char **argv) {
    int po, pi;
    gpio_mode_t mode = GPIO_IN;
    gpio_flank_t flank;
    int fl;

    if (argc < 4) {
        printf("usage: %s <port> <pin> <flank> [pull_config]\n", argv[0]);
        puts("\tflank:\n"
             "\t0: falling\n"
             "\t1: rising\n"
             "\t2: both\n"
             "\tpull_config:\n"
             "\t0: no pull resistor (default)\n"
             "\t1: pull up\n" "\t2: pull down");
        return 1;
    }

    po = atoi(argv[1]);
    pi = atoi(argv[2]);

    fl = atoi(argv[3]);
    switch (fl) {
        case 0:
            flank = GPIO_FALLING;
            break;
        case 1:
            flank = GPIO_RISING;
            break;
        case 2:
            flank = GPIO_BOTH;
            break;
        default:
            puts("error: invalid value for active flank");
            return 1;
    }

    if (argc >= 5) {
        int pr = atoi(argv[4]);
        switch (pr) {
            case 0:
                mode = GPIO_IN;
                break;
            case 1:
                mode = GPIO_IN_PU;
                break;
            case 2:
                mode = GPIO_IN_PD;
                break;
            default:
                puts("error: invalid pull resistor option");
                return 1;
        }
    }

    if (gpio_init_int(GPIO_PIN(po, pi), mode, flank, cb, (void *) pi) < 0) {
        printf("error: init_int of GPIO_PIN(%i, %i) failed\n", po, pi);
        return 1;
    }

    printf("GPIO_PIN(%i, %i) successfully initialized as ext int\n", po, pi);

    return 0;
}

static int read(int argc, char **argv) {
    int port, pin;

    if (argc < 3) {
        printf("usage: %s <port> <pin>\n", argv[0]);
        return 1;
    }

    port = atoi(argv[1]);
    pin = atoi(argv[2]);

    if (gpio_read(GPIO_PIN(port, pin))) {
        printf("GPIO_PIN(%i.%02i) is HIGH\n", port, pin);
    } else {
        printf("GPIO_PIN(%i.%02i) is LOW\n", port, pin);
    }

    return 0;
}

static int set(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <port> <pin>\n", argv[0]);
        return 1;
    }

    gpio_set(GPIO_PIN(atoi(argv[1]), atoi(argv[2])));

    return 0;
}

static int clear(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <port> <pin>\n", argv[0]);
        return 1;
    }

    gpio_clear(GPIO_PIN(atoi(argv[1]), atoi(argv[2])));

    return 0;
}

static int toggle(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <port> <pin>\n", argv[0]);
        return 1;
    }

    gpio_toggle(GPIO_PIN(atoi(argv[1]), atoi(argv[2])));

    return 0;
}

/*! \brief init_hal shell function to initialize the pHAL
 *  \warning Do not forget to call this before calling any other function
 *           of the pHAL.
**/
static int init_hal(int argc, char **argv) {
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    return pHAL_init();
}


/*! \brief movehto shell function to move the horizontal servo to
 *         the position passed in.
 *  \warning the position must be withn -90 and 90
**/
static int movehto(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: %s <angle (integer) -90..90 >\n", argv[0]);
        return 1;
    }

    int temp = atoi(argv[1]);
    printf("in move_x, temp: %d\n", temp);
    set_h(temp);

    return 0;
}

/*! \brief movevto shell function to move the vertical servo to
 *         the position passed in.
 *  \warning the position must be within -90 and 90
**/
static int movevto(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: %s <angle (integer) -90..90 >\n", argv[0]);
        return 1;
    }

    int temp = atoi(argv[1]);
    printf("in move_y, temp: %d\n", temp);
    set_v(temp);
    return 0;
}

/*! \brief left shell function to move one step to the left.
**/
static int left(int argc, char **argv) {
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    step_l();
    return 0;
}

/*! \brief right shell function to move one step to the right.
**/
static int right(int argc, char **argv) {
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    step_r();
    return 0;
}

/*! \brief up shell function to move one step upward.
**/
static int up(int argc, char **argv) {
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    step_u();
    return 0;
}

/*! \brief down shell function to move one step downward.
**/
static int down(int argc, char **argv) {
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    step_d();
    return 0;
}

/*! \brief leftn shell function to move n steps leftward.
 *  \note Will do nothing if the value passed in is <= 0.
 *  \return returns -1 if too few arguments are passed; 0 otherwise.
**/
static int leftn(int argc, char **argv) {
    if (argc < 2) {
        return -1;
    }

    int temp = atoi(argv[1]);
    printf("step n left, temp: %d\n", temp);

    stepn_l(temp);
    return 0;
}

/*! \brief rightn shell function to move n steps rightward.
 *  \note Will do nothing if the value passed it is <= 0. 
 *  \return returns -1 if too few arguments are passed; 0 otherwise.
**/
static int rightn(int argc, char **argv) {
    if (argc < 2) {
        return -1;
    }

    int temp = atoi(argv[1]);
    printf("step n right, temp: %d\n", temp);

    stepn_r(temp);
    return 0;
}

/*! \brief upn shell function to move n steps upward.
 *  \note Will do nothing if the value passed in is <= 0.
 *  \return returns -1 if too few arguments are passed; 0 otherwise.
**/
static int upn(int argc, char **argv) {
    if (argc < 2) {
        return -1;
    }

    int temp = atoi(argv[1]);
    printf("step n up, temp: %d\n", temp);

    stepn_u(temp);
    return 0;
}

/*! \brief downn shell function to move n steps downward.
 *  \note Will do nothing if the value passed in is <= 0. 
 *  \return returns -1 if too few arguments are passed; 0 otherwise.
**/
static int downn(int argc, char **argv) {
    if (argc < 2) {
        return -1;
    }

    int temp = atoi(argv[1]);
    printf("step n down, temp: %d\n", temp);

    stepn_d(temp);
    return 0;

}

/*! \brief cntrh shell function to set the horizontal servo to the center position.
**/
static int cntrh(int argc, char **argv) {
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    align_h_cntr();
    return 0;
}

/*! \brief cntrv shell function to set the vertical servo to the center position.
**/
static int cntrv(int argc, char **argv) {
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    align_v_cntr();
    return 0;
}

/*! \brief cntr shell function to center both servos.
**/
static int cntr(int argc, char **argv) {
    align_cntr();
    return 0;
}

/*! \brief sethnv shell function to set both servos at once.
 *  \return returns -1 if too few arguments were passed; 0 otherwise.
 *  \note the values pasesd must be within [500..2000]. 
**/
static int sethnv(int argc, char **argv) {
    if (argc < 3) {
        return -1;
    }

    int temp1 = atoi(argv[1]);
    int temp2 = atoi(argv[2]);
    
    printf("h: %d\n", temp1);
    printf("v: %d\n", temp2);
    sethv(temp1, temp2);
    return 0;
}

/*! \brief laseron shell function to activate the laser.
**/
static int laseron(int argc, char **argv) {
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    laser_on();
    return 0;    
}

/*! \brief laseroff shell function to deactivate the laser.
**/
static int laseroff(int argc, char **argv) {
    laser_off();
    return 0;    
}

/*! \brief lasertoggle shell function to toggle the laser.
 *
 *  If the laser is turned off calling this function will turn it on.
 *  If the laser is turned on calling this function will turn it off.
**/
static int lasertoggle(int argc, char **argv) {
    laser_toggle();
    return 0;    
}

/*! \brief demo shell function to run a little demo.
**/
static int demo(int argc, char **argv) {
    RN_UNUSED(argc);
    RN_UNUSED(argv);
    allign_cntr();
    xtimer_sleep(1);
    set_h(-90);
    xtimer_sleep(1);
    set_h(90);
    xtimer_sleep(1);
    allign_h_cntr();
    xtimer_sleep(1);
    set_v(-90);
    xtimer_sleep(1);
    set_v(90);
    xtimer_sleep(1);
    allign_v_cntr();
    xtimer_sleep(1);
    return 0;
}

/*!< array of all the shell commands */
static const shell_command_t shell_commands[] = {
    {"init_out", "init as output (push-pull mode)", init_out},
    {"init_in", "init as input w/o pull resistor", init_in},
    {"init_in_pu", "init as input with pull-up", init_in_pu},
    {"init_in_pd", "init as input with pull-down", init_in_pd},
    {"init_od", "init as output (open-drain without pull resistor)",
     init_od},
    {"init_od_pu", "init as output (open-drain with pull-up)", init_od_pu},
    {"init_int", "init as external INT w/o pull resistor", init_int},
    {"read", "read pin status", read},
    {"set", "set pin to HIGH", set},
    {"clear", "set pin to LOW", clear},
    {"toggle", "toggle pin", toggle},
    {"init_hal", "init the pHAL", init_hal},
    {"seth", "move h to n degree", movehto},
    {"setv", "move v to n degree", movevto},
    {"left", "step left", left},
    {"right", "step right", right},
    {"up", "step up", up},
    {"down", "step down", down},
    {"centerh", "allign h at cntr", cntrh},
    {"centerv", "allign v to cntr", cntrv},
    {"center", "allign h and v to cntr", cntr},
    {"leftn", "left by n", leftn},
    {"rightn", "right by n", rightn},
    {"upn", "up by n", upn},
    {"downn", "down by n", downn},
    {"sethv", "set h and v", sethnv},
    {"laseron","turn laser on", laseron},
    {"laseroff", "turn laser off", laseroff},
    {"lasertoggle", "toggle laser", lasertoggle},
    {"demo", "demo", demo},
    {NULL, NULL, NULL}
};

/*! \brief main Entry point of the pHAL application. 
**/
int main(void) {
    puts("GPIO peripheral driver test\n");
    puts("In this test, pins are specified by integer port and pin numbers.\n"
         "So if your platform has a pin PA01, it will be port=0 and pin=1,\n"
         "PC14 would be port=2 and pin=14 etc.\n\n"
         "NOTE: make sure the values you use exist on your platform! The\n"
         "      behavior for not existing ports/pins is not defined!");

    /* start the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
