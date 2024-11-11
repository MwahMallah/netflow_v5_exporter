#ifndef _PACKET_HANDLER_INFO_H
#define _PACKET_HANDLER_INFO_H

#include "../flow/flow_arr.h"
#include "../utils/exporter_info.h"

typedef struct {
    flow_arr* arr;
    exporter_info* exporter_info;
} packet_handler_info;

#endif