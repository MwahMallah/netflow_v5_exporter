#include "flow_arr.h"
#include <stdlib.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <netinet/ip.h>

flow_arr* create_flow_array() {
    flow_arr* array = malloc(sizeof(flow_arr));
    if (!array)
        return NULL;
    array->flows = malloc(INIT_FLOW_ARRAY_SIZE * sizeof(flow_record*));
    if (!array->flows) {
        free(array);
        return NULL;
    }

    array->capacity = INIT_FLOW_ARRAY_SIZE;
    array->size = 0;
    return array;
}

void add_flow(flow_arr* array, flow_record* flow) {
    //increase array size if needed
    if (array->size >= array->capacity) {
        array->capacity *= 2;
        array->flows = realloc(array->flows,
             array->capacity * sizeof(flow_record*));
    }

    array->flows[array->size++] = flow;
}

void delete_flow_array(flow_arr* array) {
    if (array != NULL) {
        // Free memory for every flow record
        for (int i = 0; i < array->size; ++i) {
            if (array->flows[i] != NULL) {
                free(array->flows[i]); // Delete flow record
            }
        }
        // Delete flow array
        free(array->flows);
        free(array);
    }
}

int is_flow_found(flow_record* flow, flow_record_filter* filter) {
    int is_atimeout_expired = (filter->systime - ntohl(flow->fpacket_systime)) > filter->atimeout;
    int is_itimeout_expired = (filter->systime - ntohl(flow->lpacket_systime)) > filter->itimeout;

    return !is_atimeout_expired && !is_itimeout_expired 
            && flow->ip_src == filter->ip_src && flow->ip_dst == filter->ip_dst
            && flow->port_src == filter->port_src && flow->port_dst == filter->port_dst;
}

flow_record* find_or_create_flow(flow_arr* arr, flow_record_filter* filter) {
    //search from the end to find last entry (the one with needed time) with given ip/port pairs
    for (int i = arr->size - 1; i >= 0; i--) {
        if (is_flow_found(arr->flows[i], filter)) {
            return arr->flows[i];
        }
    }

    flow_record* new_flow = create_flow(filter->ip_src, filter->ip_dst, 
                                        filter->port_src, filter->port_dst,
                                        filter->systime);
    if (!new_flow)
        return NULL;

    add_flow(arr, new_flow);
    return new_flow;
}
