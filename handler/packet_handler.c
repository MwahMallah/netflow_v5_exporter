#include "packet_handler.h"

// #define __USE_MISC
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <time.h>
#include <sys/sysinfo.h>

#include "../flow/flow_arr.h"
#include "../flow/flow_record.h"
#include "packet_handler_info.h"
#include "../utils/exporter_info.h"

void packet_handler(unsigned char* info, const struct pcap_pkthdr* thdr, const unsigned char* packet) {
    packet_handler_info* handler_info = (packet_handler_info*) info;
    //destructure handler information
    flow_arr* arr = handler_info->arr;
    exporter_info* exporter_info = handler_info->exporter_info;

    struct ip* iphdr = (struct ip*) (packet + 14); //Assume Ethernet header
    int ip_header_len = iphdr->ip_hl * 4;          //length of ip header
    struct tcphdr* tcphdr = (struct tcphdr*) (packet + 14 + ip_header_len); //tcp header
    
    int total_length = ntohs(iphdr->ip_len); //total Layer 3 bytes packet length

    flow_record* flow = find_or_create_flow(arr, iphdr->ip_src.s_addr, iphdr->ip_dst.s_addr, 
                            tcphdr->th_sport, tcphdr->th_dport);
                            
    //calculate system uptime at this point
    flow->octets  += total_length;
    flow->packets += 1;

    // system uptime
    struct sysinfo sys_info;
    sysinfo(&sys_info);
    unsigned long long sys_uptime = sys_info.uptime * 1000;

    // time in milliseconds
    unsigned long long packet_time = thdr->ts.tv_sec * 1000 + thdr->ts.tv_usec / 1000;
    unsigned long long unix_time = time(NULL) * 1000;
    unsigned long long unix_time_minus_packet = packet_time - unix_time + sys_uptime;

    // first and last system uptimes
    if (flow->fpacket_systime == -1) {
        flow->fpacket_systime = htonl(unix_time_minus_packet);
    }
    flow->lpacket_systime = htonl(unix_time_minus_packet + sys_uptime);
}