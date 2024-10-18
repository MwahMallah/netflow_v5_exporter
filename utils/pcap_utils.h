#ifndef __PCAP_UTILS_H
#define __PCAP_UTILS_H

#include <pcap.h>
#include "err.h"

error filter_tcp_packets(pcap_t* handler);

#endif