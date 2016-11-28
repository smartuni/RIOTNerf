#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <net/af.h>
#include <net/sock/udp.h>

static uint8_t buf[128];

int main(void)
{
    memset(buf, 0, sizeof(buf) * sizeof(*buf));
    sock_udp_ep_t local =
        SOCK_IPV6_EP_ANY;

    sock_udp_t sock;

    local.port = 12345;

    if (sock_udp_create(&sock, &local,
                        NULL, 0) < 0) 
    {
        puts("Error creating UDP socket");
        return EXIT_FAILURE;
    }

    for (;;) {
        sock_udp_ep_t remote;
        ssize_t res;

        if ((res = sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, &remote)) >= 0) 
        {
            puts("Received a message");
            puts("Here is the message:");
            printf("%s\n", buf);
            if (sock_udp_send(&sock, buf, res, &remote) < 0) 
            {
                puts("Error sending reply");
            }
        }
    } /* END for (;;) */
    return EXIT_SUCCESS;
} /* END of main */

