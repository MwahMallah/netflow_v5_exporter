#include "flow_record.h"
#include <stdlib.h>

flow_record* create_flow(int ip_src, int ip_dst, int port_src, int port_dst) {
    flow_record* new_flow = malloc(sizeof(flow_record));
    if (new_flow == NULL) {
        return NULL; // Если не удалось выделить память
    }

    new_flow->port_src = port_src;
    new_flow->port_dst = port_dst;
    new_flow->ip_src   = ip_src;
    new_flow->ip_dst   = ip_dst;
    new_flow->octets   = 0;
    new_flow->packets  = 0;
    return new_flow;
}