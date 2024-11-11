#ifndef _ERR_H
#define _ERR_H

typedef enum {
    ERR_PARSE_ARGS, ERR_PCAP_FILTER, 
    ERR_SOCKET_CREATION, ERR_SEND,
    SUCC
} error;

#endif