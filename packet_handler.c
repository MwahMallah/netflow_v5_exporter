#include "packet_handler.h"

// #define __USE_MISC
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include "flow/flow_arr.h"
#include "flow/flow_record.h"

void packet_handler(unsigned char* data, const struct pcap_pkthdr* thdr, const unsigned char* packet) {
    flow_arr* arr = (flow_arr*) data;
    struct ip* iphdr = (struct ip*) (packet + 14); //Assume Ethernet header
    int ip_header_len = iphdr->ip_hl * 4;          //length of ip header
    struct tcphdr* tcphdr = (struct tcphdr*) (packet + 14 + ip_header_len); //tcp header
    
    int total_length = ntohs(iphdr->ip_len); //total Layer 3 bytes packet length

    flow_record* flow = find_or_create_flow(arr, iphdr->ip_src.s_addr, iphdr->ip_dst.s_addr, 
                            ntohs(tcphdr->th_sport), ntohs(tcphdr->th_dport));
                            
    flow->octets  += total_length;
    flow->packets += 1;
}