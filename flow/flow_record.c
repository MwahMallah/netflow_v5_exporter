#include "flow_record.h"
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/ip.h>


flow_record* create_flow(int ip_src, int ip_dst, int port_src, int port_dst, unsigned long systime) {
    flow_record* new_flow = malloc(sizeof(flow_record));
    if (new_flow == NULL) {
        return NULL; 
    }

    new_flow->port_src = port_src;
    new_flow->port_dst = port_dst;
    new_flow->ip_src   = ip_src;
    new_flow->ip_dst   = ip_dst;
    new_flow->nexthop  = 0;        // Set default value for nexthop
    new_flow->input    = 0;        // Default SNMP index of input interface
    new_flow->output   = 0;        // Default SNMP index of output interface
    new_flow->packets  = 0;        // Initial value for packets amount
    new_flow->octets   = 0;        // Initial value for octets amount
    new_flow->fpacket_systime = htonl(systime);    // SysUptime at the first packet (set to -1 to mark not set)
    new_flow->lpacket_systime  = 0;     // SysUptime at the last packet
    new_flow->pad1     = 0;        // Padding byte
    new_flow->tcp_flags= 0;       // TCP flags
    new_flow->prot     = TCP_PROTOCOL_TYPE;// IP protocol type (Exports only TCP packets)
    new_flow->tos      = 0;        // IP type of service
    new_flow->src_as   = 0;        // Autonomous system number of the source
    new_flow->dst_as   = 0;        // Autonomous system number of the destination
    new_flow->src_mask = 0;        // Source address prefix mask 
    new_flow->dst_mask = 0;        // Destination address prefix mask bits
    new_flow->pad2[0]  = 0;        // Unused bytes
    new_flow->pad2[1]  = 0;        // Unused bytes

    return new_flow;
}