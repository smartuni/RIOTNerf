#include <stdio.h> /* puts */
#include <msg.h>
#include <xtimer.h>
#include <thread.h>
#include <shell.h>
#include <periph/gpio.h>
#include "server_conn.h"
#include "pHAL.h"

/*! \def ADDR_AUTOCONF_DELAY
 *  \brief The amount of seconds to sleep in main
 *         in order to wait for the address autoconfiguration to 
 *         be completed.
 *  \see main
**/
#define ADDR_AUTOCONF_DELAY (3)

/*! \def SERVER_THREAD_PRIORITY
 *  \brief The thread priority for the server thread.
 *
 *  The second highest priority just after the main thread.
**/
#define SERVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

/*! \def MAIN_MSG_QUEUE_SIZE
 *  \brief the amount of elements of the main message queue
**/
#define MAIN_MSG_QUEUE_SIZE (8)

/*!< the main message queue */
static msg_t _main_msg_queue[MAIN_MSG_QUEUE_SIZE];

/*!< the stack for the server thread to use */
char stack[THREAD_STACKSIZE_MAIN];

#if 0
static const shell_command_t shell_commands[] = {
    { },
    { NULL, NULL, NULL }
};
#endif /* 0 */

/*! \brief _netif_config TODO: add documentation.
 *  \param argc TODO: add documentation.
 *  \param argv TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  TODO: add detailed documentation.
**/
int _netif_config(int argc, char **argv);

/*! \brief main the entry point of the coaptest application.
**/
int main(void) {
    puts("COAP SERVER TEST");
    puts("!!! JUST SERVER !!!");

    /* initialize the pHAL and center both the servos to start out */
    pHAL_init();
    allign_cntr();

    //gpio_init(GPIO_PIN(PORT_A, 0), GPIO_OUT);
    //gpio_set(GPIO_PIN(PORT_A, 0));

    /* create the main message queue */
    msg_init_queue(_main_msg_queue, MAIN_MSG_QUEUE_SIZE);

    puts("Waiting for address autoconfiguration...");
    xtimer_sleep(ADDR_AUTOCONF_DELAY);

    puts("Configured network interfaces:");
    _netif_config(0, NULL);

    /* launch the server */
    thread_create(stack, sizeof(stack),
                  SERVER_THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, &server_loop, NULL, "thread");
    puts("Server thread started!");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
    puts("Shell started!");
    //server_loop();

    for (;;) {
    }

    return 0;
}
