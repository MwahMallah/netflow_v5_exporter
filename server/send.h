#ifndef _SEND_H
#define _SEND_H

#include "../handler/packet_handler_info.h"
#include "../utils/err.h"

error send_netflow(packet_handler_info* handler_info);

#endif