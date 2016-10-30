#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pktdump.h"
#include "shell.h"
#include "xtimer.h"

#include "the_thread.h"

int main(void)
{
    kernel_pid_t thd = get_thread();

    gnrc_netreg_entry_t dump = { NULL, GNRC_NETREG_DEMUX_CTX_ALL,
                                 { thd } };
    gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &dump);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
