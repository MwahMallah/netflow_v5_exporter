#include "send.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <string.h>

#include "../flow/flow_arr.h"
#include "netflow_header.h"

void fill_netflow_header(netflow_header* header);
void set_server_addr(struct sockaddr_in* addr, exporter_info* info);
error send_netflow_msg(int fd, struct sockaddr_in* server_addr, char* msg, int len);

error send_netflow(packet_handler_info* handler_info) {
    //create udp socket and address
    struct sockaddr_in server_addr;
    set_server_addr(&server_addr, handler_info->exporter_info);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); 

    if (sockfd < 0) 
        return ERR_SOCKET_CREATION;
    //create netflow msg
    flow_arr* flow_arr = handler_info->arr;
    netflow_header header;
    fill_netflow_header(&header);

    char netflow_msg[sizeof(netflow_header) + MAX_FLOWS * sizeof(flow_record)];
    char* current_pos = (netflow_msg + sizeof(netflow_header));

    for (int i = 0; i < flow_arr->size; i++) {
        header.flows++;
        flow_record* flow = flow_arr->flows[i];
        flow->octets  = htonl(flow->octets);
        flow->packets = htonl(flow->packets);
        //push new flow record to netflow message
        memcpy(current_pos, flow, sizeof(flow_record));        
        //update to new position in a message
        current_pos += sizeof(flow_record);
    }
    //prepare flows to be in network byte order
    header.flows = htons(header.flows);
    //push header at the beginning of the netflow_msg
    memcpy(netflow_msg, &header, sizeof(header));

    int msg_len = current_pos - netflow_msg; //length of the message
    if (send_netflow_msg(sockfd, &server_addr, netflow_msg, msg_len) == ERR_SEND) {
        perror("send");
        close(sockfd);
        return ERR_SEND;
    }
    
    close(sockfd);
    return SUCC;
}

void set_server_addr(struct sockaddr_in* addr, exporter_info* info) {
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(info->host);
    addr->sin_port = htons(info->port);
}

void fill_netflow_header(netflow_header* header) {
    header->version = htons(5); //use version 5 of netflow
    header->flows = 0; //set flows at 0 at the beginning

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    struct sysinfo sys_info;
    sysinfo(&sys_info);

    header->uptime_ms = htonl(sys_info.uptime * 1000); //system uptime in ms
    header->time_sec = htonl(ts.tv_sec);       // Unix seconds
    header->time_nanosec = htonl(ts.tv_nsec);  // Unix nanoseconds

    header->flow_sequence = 0; //set flow sequence at 0 at the beginning

    // Not set
    header->engine_type = 0;  
    header->engine_id = 0;    
    header->sampling_interval = htons(0);
}

error send_netflow_msg(int fd, struct sockaddr_in* server_addr, char* msg, int len) {
     if (sendto(fd, msg, len, 0, (struct sockaddr*)server_addr, sizeof(*server_addr)) < 0) {
        return ERR_SEND;
     }
     
     return SUCC;
}
