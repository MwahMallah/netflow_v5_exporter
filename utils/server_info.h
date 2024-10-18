#ifndef __ARG_PARSE_H
#define __ARG_PARSE_H

#include "err.h"

#define DEFAULT_ACTIVE_TIMEOUT 60
#define DEFAULT_INACTIVE_TIMEOUT 60

typedef struct exporter_info_t {
    char* host;
    int port;
    char* pcap_file;
    int active_timeout;
    int inactive_timeout;
} exporter_info;

error get_info_from_cla(int argc, char** argv, exporter_info* info);

#endif