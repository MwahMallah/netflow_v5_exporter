#ifndef _FLOW_RECORD_H
#define _FLOW_RECORD_H

#include <stdint.h>

typedef struct {
    uint32_t ip_src;         // Source IP address
    uint32_t ip_dst;         // Destination IP address
    uint32_t nexthop;        // IP address of next hop router (можно оставить 0, если не используется)
    uint16_t input;          // SNMP index of input interface
    uint16_t output;         // SNMP index of output interface
    uint32_t packets;        // Packets in the flow
    uint32_t octets;         // Total number of Layer 3 bytes in the packets of the flow
    uint32_t first;          // SysUptime at start of flow
    uint32_t last;           // SysUptime at the time the last packet of the flow was received
    uint16_t port_src;       // TCP/UDP source port number or equivalent
    uint16_t port_dst;       // TCP/UDP destination port number or equivalent
    uint8_t pad1;            // Unused (zero) byte
    uint8_t tcp_flags;       // Cumulative OR of TCP flags
    uint8_t prot;            // IP protocol type (for example, TCP = 6; UDP = 17)
    uint8_t tos;             // IP type of service (ToS)
    uint16_t src_as;         // Autonomous system number of the source, either origin or peer
    uint16_t dst_as;         // Autonomous system number of the destination, either origin or peer
    uint8_t src_mask;        // Source address prefix mask bits
    uint8_t dst_mask;        // Destination address prefix mask bits
    uint8_t pad2[2];         // Unused (zero) bytes
} flow_record;

//Create new flow
flow_record* create_flow(int ip_src, int ip_dst, int port_src, int port_dst);

#endif