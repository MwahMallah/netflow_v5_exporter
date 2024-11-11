#ifndef _NETFLOW_HEADER_H
#define _NETFLOW_HEADER_H

#include <stdint.h>

#define MAX_FLOWS 30

typedef struct {
  uint16_t version;
  uint16_t flows;     
  uint32_t uptime_ms;  
  uint32_t time_sec;
  uint32_t time_nanosec;
  uint32_t flow_sequence;
  uint8_t engine_type;
  uint8_t engine_id;
  uint16_t sampling_interval;
} netflow_header;

#endif