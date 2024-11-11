#ifndef _PACKET_HANDLER_H
#define _PACKET_HANDLER_H

#include <pcap.h>

void packet_handler(unsigned char* info, const struct pcap_pkthdr* thdr, const unsigned char* packet);

#endif