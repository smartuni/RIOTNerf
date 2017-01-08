#include <net/af.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#include "coap.h"
#include "coaptarget.h"

#define COAP_SERVER_PORT_TARGET (5683)
#define COAP_SERVER_PORT (5683)

static uint8_t _udp_buf[512];

static const sock_udp_ep_t local  = { .family = AF_INET6, .port = COAP_SERVER_PORT_TARGET };
static const sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_SERVER_PORT_TARGET };

static sock_udp_t sock;

static coap_pkt_t coappkt;

static uint16_t idnum = 0x3322;

void initcoaptarget(char* addrstr, char* portstr) {
	char* uripath = "target/hit";
	uint8_t uripathlen = strlen(uripath);

	char* resourcevalue = "1";
	uint8_t resourcevaluelen = strlen(resourcevalue);

	ipv6_addr_t addr;
	uint16_t port;

	ipv6_addr_from_str(&addr, addrstr); // ADD CHECK HERE
	port = (uint16_t)atoi(portstr); // ADD CHECK HERE

	coap_pkt_t coappkt;


	coappkt.hdr.ver = 0x01;
	coappkt.hdr.t = COAP_TYPE_CON;
	coappkt.hdr.tkl = 0;
	coappkt.hdr.code = COAP_METHOD_POST;
	coappkt.hdr.id[0] = (uint8_t)idnum & 0xFF;
	coappkt.hdr.id[1] = ((uint8_t)idnum >> 8) & 0xFF;

	coappkt.numopts = 1;

	coappkt.opts[0].num = COAP_OPTION_URI_PATH;
	coappkt.opts[0].buf.p = uripath;
	coappkt.opts[0].buf.len = uripathlen;

	coappkt.payload.p = resourcevalue;
	coappkt.payload.len = resourcepathlen;


	uint8_t rsplen = sizeof(_udp_buf);
	coap_build(_udp_buf, &rsplen, &coappkt);

	ipv6_addr_set_all_nodes_multicast((ipv6_addr_t *) &remote.addr.ipv6, IPV6_ADDR_MCAST_SCP_LINK_LOCAL);
}

void sendGotHit(void) {


	rc = sock_udp_send(&sock, _udp_buf, rsplen, &remote);

	coappkt.hdr.id[0] = (uint8_t)idnum & 0xFF;
	coappkt.hdr.id[1] = ((uint8_t)idnum >> 8) & 0xFF;

	idnum++;

}
