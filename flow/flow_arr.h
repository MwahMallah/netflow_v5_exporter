#ifndef _FLOW_ARR_H
#define _FLOW_ARR_H

#include "flow_record.h"

#define INIT_FLOW_ARRAY_SIZE 2

typedef struct {
    flow_record **flows;
    int size;    // Текущий размер массива
    int capacity; // Вместимость массива
} flow_arr;

typedef struct {
    int ip_src;
    int ip_dst;
    int port_src;
    int port_dst;
    uint32_t systime;
    int itimeout;
    int atimeout;
} flow_record_filter;

//Create and initialize new flow array
flow_arr* create_flow_array();
//Add new flow to flow array
void add_flow(flow_arr* array, flow_record* flow);
//Delete flow array
void delete_flow_array(flow_arr* array);
//Finds or creates record with given src/dst ip and given src/dst port
flow_record* find_or_create_flow(flow_arr* arr, flow_record_filter* params);

#endif