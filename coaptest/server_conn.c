#include <stdio.h>

#include "net/af.h"
#include "net/conn/udp.h"

#define ENABLE_DEBUG (1)

#include "debug.h"
#include "coap.h"
#include "pHAL.h"

#define RN_UNUSED(arg)  ((void) (arg))

static uint8_t _udp_buf[512];
uint8_t scratch_raw[1024];

coap_rw_buffer_t scratch_buf = { scratch_raw, sizeof(scratch_raw) };

#define MAIN_MSG_QUEUE_SIZE 8
static msg_t _main_msg_queue[MAIN_MSG_QUEUE_SIZE];

#define COAP_SERVER_PORT (5683)

void *server_loop(void *arg)
{
    RN_UNUSED(arg);

    DEBUG("Server Loop started!");
    puts("");

    msg_init_queue(_main_msg_queue, MAIN_MSG_QUEUE_SIZE);

    uint8_t laddr[16] = { 0 };
    uint8_t raddr[16] = { 0 };
    size_t raddr_len;
    uint16_t rport;

    DEBUG("Creating connection...");
    puts("");

    conn_udp_t conn;

    int rc =
	conn_udp_create(&conn, laddr, sizeof(laddr), AF_INET6,
			COAP_SERVER_PORT);

    while (1) {
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
    }

    return NULL;
}
