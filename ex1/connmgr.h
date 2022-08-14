#include "lib/dplist.h"
#include "lib/tcpsock.h"
#include "config.h"

#include <stdio.h>
#include <poll.h>
#include <stdlib.h>

typedef struct pollfd pollReqDescr;

typedef struct{
    pollReqDescr file_descriptors;
    time_t last_record;
    sensor_id_t sensor_id;
    tcpsock_t* socket_id;
} pollData;

void connmgr_listen(int port_number);

void connmgr_free();
