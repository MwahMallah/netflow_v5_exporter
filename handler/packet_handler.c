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

flow_record* get_flow(flow_arr* arr, struct ip* iphdr, struct tcphdr* tcphdr, uint32_t systime, exporter_info* exporter_info);
uint32_t get_systime(unsigned long packet_time);

void packet_handler(unsigned char* info, const struct pcap_pkthdr* thdr, const unsigned char* packet) {
    packet_handler_info* handler_info = (packet_handler_info*) info;
    //destructure handler information
    flow_arr* arr = handler_info->arr;
    exporter_info* exporter_info = handler_info->exporter_info;

    struct ip* iphdr = (struct ip*) (packet + 14); //Assume Ethernet header
    int ip_header_len = iphdr->ip_hl * 4;          //length of ip header
    struct tcphdr* tcphdr = (struct tcphdr*) (packet + 14 + ip_header_len); //tcp header
    int total_length = ntohs(iphdr->ip_len); //total Layer 3 bytes packet length

    uint32_t systime = get_systime(thdr->ts.tv_sec * 1000 + thdr->ts.tv_usec / 1000);
    flow_record* flow = get_flow(arr, iphdr, tcphdr, systime, exporter_info);

    flow->octets  += total_length;
    flow->packets += 1;

    flow->lpacket_systime = htonl(systime);
}

flow_record* get_flow(flow_arr* arr, struct ip* iphdr, struct tcphdr* tcphdr, uint32_t systime, exporter_info* exporter_info) {
    flow_record_filter filter = {
        .ip_src = iphdr->ip_src.s_addr,
        .ip_dst = iphdr->ip_dst.s_addr,
        .port_src = tcphdr->th_sport,
        .port_dst = tcphdr->th_dport,
        .systime = systime,
        .itimeout = exporter_info->inactive_timeout,
        .atimeout = exporter_info->active_timeout
    };

    return find_or_create_flow(arr, &filter);
}

uint32_t get_systime(unsigned long packet_time) {
    // system uptime
    struct sysinfo sys_info;
    sysinfo(&sys_info);
    long sys_uptime = sys_info.uptime * 1000;

    long unix_time = time(NULL) * 1000;
    return packet_time - unix_time + sys_uptime;
}