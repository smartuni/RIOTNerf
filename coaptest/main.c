#include <stdio.h>

#include "msg.h"
#include "xtimer.h"

#define MAIN_MSG_QUEUE_SIZE 8
static msg_t _main_msg_queue[MAIN_MSG_QUEUE_SIZE];

void server_loop(void);

extern int _netif_config(int argc, char **argv);

int main(void) {
    puts("COAP SERVER TEST");
    puts("!!! JUST SERVER !!!");

    msg_init_queue(_main_msg_queue, MAIN_MSG_QUEUE_SIZE);

    puts("Waiting for address autoconfiguration...");
    xtimer_sleep(3);

    puts("Configured network interfaces:");
    _netif_config(0, NULL);

    server_loop();

    puts("After Server Loop. WTF?");

    return 0;
}
