#include <stdio.h>

#include "msg.h"
#include "xtimer.h"
#include "thread.h"
#include "shell.h"
#include "periph/gpio.h"

#include "server_conn.c"
#include "coap.c"
#include "pHAL.h"

#define MAIN_MSG_QUEUE_SIZE 8
static msg_t _main_msg_queue[MAIN_MSG_QUEUE_SIZE];

char stack[THREAD_STACKSIZE_MAIN];

/* static const shell_command_t shell_commands[] = {
    {},
    { NULL, NULL, NULL }
}; */

//void server_loop(void); // server_conn.c

extern int _netif_config(int argc, char **argv);

int main(void)
{
    puts("COAP SERVER TEST");
    puts("!!! JUST SERVER !!!");

    pHAL_init();
    allign_cntr();

    //gpio_init(GPIO_PIN(PORT_A, 0), GPIO_OUT);
    //gpio_set(GPIO_PIN(PORT_A, 0));

    msg_init_queue(_main_msg_queue, MAIN_MSG_QUEUE_SIZE);

    puts("Waiting for address autoconfiguration...");
    xtimer_sleep(3);

    puts("Configured network interfaces:");
    _netif_config(0, NULL);

    thread_create(stack, sizeof(stack),
		  THREAD_PRIORITY_MAIN - 1,
		  THREAD_CREATE_STACKTEST, server_loop, NULL, "thread");
    puts("Server thread started!");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
    puts("Shell started!");
    //server_loop();

    while (1) {
    }

    return 0;
}
