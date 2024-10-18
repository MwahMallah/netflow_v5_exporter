#ifndef _PACKET_HANDLER_H
#define _PACKET_HANDLER_H

#include <pcap.h>

void packet_handler(unsigned char* user_data, const struct pcap_pkthdr* thdr, const unsigned char* packet);

#endif