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

int is_flow_found(flow_record* flow, int ip_src, int ip_dst) {
    return flow->ip_src == ip_src && flow->ip_dst == ip_dst;
}


flow_record* find_or_create_flow(flow_arr* arr, int ip_src, int ip_dst, int port_src, int port_dst) {
    for (int i = 0; i < arr->size; i++) {
        if (is_flow_found(arr->flows[i], ip_src, ip_dst)) {
            return arr->flows[i];
        }
    }

    flow_record* new_flow = create_flow(ip_src, ip_dst, port_src, port_dst);
    if (!new_flow)
        return NULL;

    add_flow(arr, new_flow);
    return new_flow;
}
