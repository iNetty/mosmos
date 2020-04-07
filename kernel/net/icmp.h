#ifndef _ICMP_H_
#define _ICMP_H_

#include <stdint.h>
#include "pktbuf.h"

#define ICMP_HDR_TYPE_ECHO_REPLY 0
#define ICMP_HDR_TYPE_ECHO_REQUEST 8

struct icmp_hdr {
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
};

struct icmp_echo {
	uint16_t id;
	uint16_t seqnum;
};

void icmp_rx(struct pktbuf *pkt, uint32_t sip);
void icmp_tx(uint32_t dip);

#endif
