#ifndef _FLOW_RECORD_H
#define _FLOW_RECORD_H

#include <stdint.h>

#define TCP_PROTOCOL_TYPE 6

typedef struct {
    uint32_t ip_src;         // Source IP address
    uint32_t ip_dst;         // Destination IP address
    uint32_t nexthop;        // Next hop router (!unused!)
    uint16_t input;          // SNMP index of input interface
    uint16_t output;         // SNMP index of output interface
    uint32_t packets;        // Packets in the flow
    uint32_t octets;         // Total number of Layer 3 bytes in the packets of the flow
    uint32_t fpacket_systime;    // SysUptime at start of flow (first packet)
    uint32_t lpacket_systime;     // SysUptime at the end of flow (last packet)
    uint16_t port_src;       // TCP source port number
    uint16_t port_dst;       // TCP destination port number
    uint8_t pad1;            // Unused (zero) byte
    uint8_t tcp_flags;       // TCP flags
    uint8_t prot;            // IP protocol type (for example, TCP = 6; UDP = 17)
    uint8_t tos;             // IP type of service (ToS)
    uint16_t src_as;         // Number of the source (!unused!)
    uint16_t dst_as;         // Number of the destination (!unused!)
    uint8_t src_mask;        // Source address prefix mask bits (!unused!)
    uint8_t dst_mask;        // Destination address prefix mask bits (!unused!)
    uint8_t pad2[2];         // Unused (zero) bytes
} flow_record;

//Create new flow
flow_record* create_flow(int ip_src, int ip_dst, int port_src, int port_dst, unsigned long systime);

#endif