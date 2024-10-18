#include "server_info.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <bits/getopt_core.h>

error parse_port(const char* str, int* value) {
    char *endptr;
    errno = 0;
    int val = strtol(str, &endptr, 10);

    if (errno != 0) {
        perror("strtol");
        return ERR_PARSE_ARGS;
    }

    if (endptr == str) {
        fprintf(stderr, "No digits were found\n");
        return ERR_PARSE_ARGS;
    }

    *value = val;
    return SUCC;
}

error get_info_from_cla(int argc, char** argv, exporter_info* info) {
    if (argc < 3 || argc > 7) 
        return ERR_PARSE_ARGS;

    char* host_port = argv[1];
    char* host = strtok(host_port, ":");
    if (!host) 
        return ERR_PARSE_ARGS;
    char* port_s = strtok(NULL, ":");
    if (!port_s)
        return ERR_PARSE_ARGS;

    int port;
    error port_parse_res = parse_port(port_s, &port);
    if (port_parse_res == ERR_PARSE_ARGS) {
        printf("Port is not a number\n");
        return ERR_PARSE_ARGS;
    }
    
    char* pcap_file_path = argv[2];
    int active_timeout = DEFAULT_ACTIVE_TIMEOUT;
    int inactive_timeout = DEFAULT_INACTIVE_TIMEOUT;

    int opt;
    while ((opt = getopt(argc, argv, "a:i:")) != -1) {
        switch (opt) {
            case 'a':
                active_timeout = atoi(optarg);
                break;
            case 'i':
                inactive_timeout = atoi(optarg);
                break;
            default:
                return ERR_PARSE_ARGS;
        }
    }

    #ifdef DEBUG 
    printf("Server info: %s:%d\n", host, port);
    printf("File: %s\n", pcap_file_path);
    printf("Args: -a = %d, -i = %d\n", active_timeout, inactive_timeout);
    #endif

    info->host = host;
    info->port = port;
    info->pcap_file = pcap_file_path;
    info->active_timeout = active_timeout;
    info->inactive_timeout = inactive_timeout;

    return SUCC;
}