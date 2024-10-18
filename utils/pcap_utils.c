#include "pcap_utils.h"

error filter_tcp_packets(pcap_t* handler) {
    struct bpf_program fp;
    char filter_exp[] = "tcp";  
    if (pcap_compile(handler, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        fprintf(stderr, "Error compiling filter: %s\n", pcap_geterr(handler));
        return ERR_PCAP_FILTER;
    }

    if (pcap_setfilter(handler, &fp) == -1) {
        fprintf(stderr, "Error setting filter: %s\n", pcap_geterr(handler));
        return ERR_PCAP_FILTER;
    }

    pcap_freecode(&fp);
    return SUCC;
}
