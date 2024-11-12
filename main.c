#include <stdio.h>
#include <pcap.h>

#include "utils/exporter_info.h"
#include "utils/err.h"
#include "utils/pcap_utils.h"

#include "flow/flow_record.h"
#include "flow/flow_arr.h"

#include "handler/packet_handler_info.h"
#include "handler/packet_handler.h"

#include "server/send.h"

int main(int argc, char** argv) {
    exporter_info info;
    if (get_exporter_info_from_cla(argc, argv, &info) == ERR_PARSE_ARGS) {
        printf("Usage: ./p2nprobe <host>:<port> <pcap_file_path> [-a <active_timeout> -i <inactive_timeout>]");
        return 1;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handler = pcap_open_offline(info.pcap_file, errbuf);
    if (!handler) {
        fprintf(stderr, "Can't open %s\n", info.pcap_file);
        return 2;
    }

    //set filter to catch only tcp packets
    if (filter_tcp_packets(handler) == ERR_PCAP_FILTER) {
        fprintf(stderr, "Error making filter");
        return 3;
    }

    flow_arr* arr = create_flow_array();
    if (!arr) {
        perror("Flow array memory");
        return 4;
    }
    
    //prepare information for packet handler
    packet_handler_info handler_info = {.arr = arr, .exporter_info = &info};

    pcap_loop(handler, -1, packet_handler, (unsigned char*) &handler_info);

    #ifdef DEBUG
    for (int i = 0; i < arr->size; i++) {
        printf("%d. Flow - packets: %d, bytes: %d", i, arr->flows[i]->packets, arr->flows[i]->octets);
        printf(" First SysUptime: %u, Last SysUptime: %u\n", arr->flows[i]->fpacket_systime, arr->flows[i]->lpacket_systime);
    }
    #endif

    error send_res = send_netflow(&handler_info);

    //free all the memory, and set exit code
    delete_flow_array(arr);
    pcap_close(handler);

    if (send_res == ERR_SOCKET_CREATION) {
        fprintf(stderr, "Can't create socket\n");
        return 5;
    }

    if (send_res == ERR_SEND) {
        fprintf(stderr, "Can't send to address\n");
        return 6;
    }

    return 0;
}