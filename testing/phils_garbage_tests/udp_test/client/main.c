#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/protnum.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>

static uint8_t buf[7];

int main(void)
{
    sock_udp_ep_t local =
        SOCK_IPV6_EP_ANY;

    sock_udp_t sock;

    local.port = 0xabcd;

    if (sock_udp_create(&sock, &local,
                        NULL, 0) < 0) 
    {
        puts("Error creating UDP socket");
        return EXIT_FAILURE;
    }

    for (;;)
    {
        sock_udp_ep_t remote =
            { .family = AF_INET6,
              .port = 12345
            };
        ssize_t res;

        static char const ary[] = "Hello!";

        ipv6_addr_set_all_nodes_multicast((ipv6_addr_t *) &remote.addr.ipv6,
        IPV6_ADDR_MCAST_SCP_LINK_LOCAL);

        if (sock_udp_send(&sock, ary, sizeof(ary), &remote) < 0)
        {
            puts("Error sending message");
            sock_udp_close(&sock);
            return EXIT_FAILURE;
        }
        else
        {
            printf("Send message: %s\n", ary);
        }

        if ((res = sock_udp_recv(&sock, buf, sizeof(buf), 1 * SEC_IN_USEC, NULL)) < 0)
        {
            if (res == -ETIMEDOUT)
            {
                puts("Timed out");
            }
            else
            {
                puts("Error receiving message");
            }
        }
        else
        {
            printf("Received message: \"");
            for (int i = 0; i < res; ++i)
            {
                printf("%c", buf[i]);
            }
            printf("\"\n");
        }
        xtimer_sleep(1);
    } /* END for (;;) */
    return EXIT_SUCCESS;
} /* END of main */

