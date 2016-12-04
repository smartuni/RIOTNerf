#include <stdio.h> /* puts */
#include <stddef.h> /* size_t */
#include <stdint.h> /* uint8_t, uint16_t */
#include "net/af.h"
#include "net/conn/udp.h"

#define ENABLE_DEBUG (1)

#include "debug.h"
#include "coap.h"
#include "pHAL.h"

/*! \def RN_UNUSED(arg)
 *  \brief Macro used to mark a parameter as unused.
 *  \param arg The parameter to mark as unused.
 *  
 *  Used to suppress compiler warnings, because of unused parameters.
**/
#define RN_UNUSED(arg)  ((void) (arg))

/*! \def MAIN_MSG_QUEUE_SIZE
 *  \brief The amount of elements of the main message queue.
**/
#define MAIN_MSG_QUEUE_SIZE (8)

/*! \def IPV6_ADDRESS_SIZE 
 *  \brief The byte size of an IPv6 address.
**/
#define IPV6_ADDRESS_SIZE (16)

#define COAP_SERVER_PORT (5683)

enum {
    kiloByte = 1024,
    halfAkiloByte = kiloByte / 2
};

static uint8_t _udp_buf[halfAkiloByte];
static uint8_t scratch_raw[kiloByte];

static coap_rw_buffer_t scratch_buf = { scratch_raw, sizeof(scratch_raw) };

static msg_t _main_msg_queue[MAIN_MSG_QUEUE_SIZE];

void *server_loop(void *arg) {
    RN_UNUSED(arg);

    DEBUG("Server Loop started!");
    puts("");

    msg_init_queue(_main_msg_queue, MAIN_MSG_QUEUE_SIZE);

    uint8_t laddr[IPV6_ADDRESS_SIZE];
    uint8_t raddr[IPV6_ADDRESS_SIZE];
    size_t raddr_len;
    uint16_t rport;

    DEBUG("Creating connection...");
    puts("");

    conn_udp_t conn;

    int rc = conn_udp_create(&conn, laddr, sizeof(laddr), AF_INET6,
                             COAP_SERVER_PORT);

    for (;;) {
        DEBUG("Waiting for UDP packet...");
        puts("");
        rc = conn_udp_recvfrom(&conn, (char *) _udp_buf, sizeof(_udp_buf),
                               raddr, &raddr_len, &rport);
        if (rc < 0) {
            DEBUG("ERROR in udp_recvfrom! rc=%u", rc);
            puts("");
            continue;
        }

        size_t n = rc;

        coap_packet_t pkt;
        DEBUG("Packet received:");
        puts("");
        coap_dump(_udp_buf, n, true);

        if (0 != (rc = coap_parse(&pkt, _udp_buf, n))) {
            DEBUG("Bad packet! rc=%d", rc);
            puts("");
        } else {
            coap_packet_t rsppkt;
            DEBUG("Content:");
            puts("");
            coap_dumpPacket(&pkt);

            coap_handle_req(&scratch_buf, &pkt, &rsppkt);

            size_t rsp_len = sizeof(_udp_buf);
            if ((rc = coap_build(_udp_buf, &rsp_len, &rsppkt)) != 0) {
                DEBUG("coap_build failed! rc=%d", rc);
                puts("");
            } else {
                DEBUG("Sending packet:");
                puts("");
                coap_dump(_udp_buf, rsp_len, true);
                DEBUG("Content:");
                puts("");
                coap_dumpPacket(&rsppkt);

                rc = conn_udp_sendto(_udp_buf, rsp_len, NULL, 0, raddr,
                                     raddr_len, AF_INET6, COAP_SERVER_PORT,
                                     rport);
                if (rc < 0) {
                    DEBUG("ERROR sending CoAP reply via udp! rc=%u", rc);
                    puts("");
                }
            }
        }
    } /* END for (;;) */

    return NULL;
}
